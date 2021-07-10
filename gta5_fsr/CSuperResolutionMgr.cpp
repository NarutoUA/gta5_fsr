#include "stdafx.h"

#include "CSuperResolutionMgr.h"

#include "shaders/fsr_easu_cs_d3d.h"
#include "shaders/fsr_rcas_cs_d3d.h"

constexpr auto GTA5_FSR_EPSILON = 0.01;
extern gta5_fsr_cfg_t g_Cfg;

CSuperResolutionMgr& CSuperResolutionMgr::instance()
{
    static CSuperResolutionMgr instance;
    return instance;
}

void CSuperResolutionMgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    instance().InternalInitialize(pDevice, pDeviceContext);
}

bool CSuperResolutionMgr::RunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView> src, wil::com_ptr_t<ID3D11RenderTargetView> dst, bool fxaa)
{
    return instance().InternalRunSuperResolutionPass(std::move(src), std::move(dst), fxaa);
}

void CSuperResolutionMgr::OnDeviceLost()
{
    instance().InternalOnDeviceLost();
}

CSuperResolutionMgr::~CSuperResolutionMgr()
{
    InternalOnDeviceLost();
}

void CSuperResolutionMgr::InternalInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    m_pDevice = pDevice;
    m_pDeviceContext = pDeviceContext;
    m_cbRCAS.cSharpness = GTA5_FSR_DEFAULT_SHARPNESS;

    ZeroMemory(&m_cbEASU, sizeof(m_cbEASU));

    CreateResources();

    print_debug("GTA5_FSR: Initialized");
}

bool CSuperResolutionMgr::InternalRunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView>&& src, wil::com_ptr_t<ID3D11RenderTargetView>&& dst, bool fxaa)
{
    HandleInputKeys();

    if (g_Cfg.UseFidelityFxSuperResolution == false || m_bResourcesAvailable == false || src == nullptr || dst == nullptr)
        return false;

    // Check if this is original upscale pass
    D3D11_SHADER_RESOURCE_VIEW_DESC src_srv_desc;
    src->GetDesc(&src_srv_desc);
    if (src_srv_desc.ViewDimension != D3D11_SRV_DIMENSION_TEXTURE2D || src_srv_desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
        return false;

    D3D11_RENDER_TARGET_VIEW_DESC dst_rtv_desc;
    dst->GetDesc(&dst_rtv_desc);
    if (dst_rtv_desc.ViewDimension != D3D11_RTV_DIMENSION_TEXTURE2D || dst_rtv_desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
        return false;

    wil::com_ptr_t<ID3D11Texture2D> src_tex;
    src->GetResource(reinterpret_cast<ID3D11Resource**>(src_tex.addressof()));
    if (src_tex == nullptr)
        return false;

    wil::com_ptr_t<ID3D11Texture2D> dst_tex;
    dst->GetResource(reinterpret_cast<ID3D11Resource**>(dst_tex.addressof()));
    if (dst_tex == nullptr)
        return false;

    D3D11_TEXTURE2D_DESC src_tex_desc;
    src_tex->GetDesc(&src_tex_desc);
    auto src_width = static_cast<float>(src_tex_desc.Width);
    auto src_height = static_cast<float>(src_tex_desc.Height);
    auto src_ratio = src_width / src_height;

    D3D11_TEXTURE2D_DESC dst_tex_desc;
    dst_tex->GetDesc(&dst_tex_desc);
    auto dst_width = static_cast<float>(dst_tex_desc.Width);
    auto dst_height = static_cast<float>(dst_tex_desc.Height);
    auto dst_ratio = dst_width / dst_height;

    auto frame_scale = src_width / dst_width;

    if (src_tex_desc.Width >= dst_tex_desc.Width || src_tex_desc.Height >= dst_tex_desc.Height
        || std::abs(src_ratio - dst_ratio) > GTA5_FSR_EPSILON
        || frame_scale < (GTA5_MIN_FRAME_SCALE - GTA5_FSR_EPSILON))
        return false;

    DetectMSAA();

    if (UpdateResources(src_tex_desc, dst_tex_desc) == false)
        return false;

    // Do original FXAA before FSR
    if (fxaa)
    {
        m_pDeviceContext->OMSetRenderTargets(1, m_pRtvFXAA.addressof(), nullptr);

        D3D11_VIEWPORT vp;
        ZeroMemory(&vp, sizeof(vp));
        vp.Width = static_cast<FLOAT>(src_tex_desc.Width);
        vp.Height = static_cast<FLOAT>(src_tex_desc.Height);
        vp.MaxDepth = 1.f;

        m_pDeviceContext->RSSetViewports(1, &vp);
        m_pDeviceContext->Draw(3, 3);
        
        vp.Width = static_cast<FLOAT>(dst_tex_desc.Width);
        vp.Height = static_cast<FLOAT>(dst_tex_desc.Height);
        m_pDeviceContext->RSSetViewports(1, &vp);
        m_pDeviceContext->OMSetRenderTargets(1, dst.addressof(), nullptr);
    }

    // Run FidelityFx Super Resolution pass
    if (FAILED(RunComputeShader(m_pDeviceContext, m_pShaderEASU.get(), fxaa ? m_pSrvFXAA.get() : src.get(), m_pBufEASU.get(), m_pUavEASU.get(), m_pSampler.get(), m_ThreadGroupsXY.first, m_ThreadGroupsXY.second, 1)))
        return false;

    if (FAILED(RunComputeShader(m_pDeviceContext, m_pShaderRCAS.get(), m_pSrvRCAS.get(), m_pBufRCAS.get(), m_pUavRCAS.get(), nullptr, m_ThreadGroupsXY.first, m_ThreadGroupsXY.second, 1)))
        return false;

    if (m_MSAA)
    {
        m_pDeviceContext->CopyResource(dst_tex.get(), m_pTexRCAS.get());
    }
    else
    {
        wil::com_ptr_t<ID3D11ShaderResourceView> pSrv;
        CreateTextureSRV(m_pDevice, m_pTexRCAS.get(), pSrv.addressof());
        m_pDeviceContext->PSSetShaderResources(fxaa ? GTA5_CB_SLOT_POSTFX_FXAA : GTA5_CB_SLOT_POSTFX, 1, pSrv.addressof());
        m_pDeviceContext->Draw(3, 3);
    }

    static ULONGLONG tick = 0;
    static auto fps = 0;
    if (print_debug_timeout(tick, 1000
        , "GTA5_FSR: FSR PASS, FPS: %d, MSAA: %d, FXAA: %d, [%dx%d]->[%dx%d] (%f)"
        , fps++, m_MSAA, fxaa, src_tex_desc.Width, src_tex_desc.Height, dst_tex_desc.Width, dst_tex_desc.Height, m_fSharpness)
        )
        fps = 0;

    return true;
}

void CSuperResolutionMgr::InternalOnDeviceLost()
{
    print_debug("GTA5_FSR: Device lost");
    ReleaseResources();
}

void CSuperResolutionMgr::ReleaseResources()
{
    m_bResourcesAvailable = false;

    m_pDevice = nullptr;
    m_pDeviceContext = nullptr;

    m_pSampler.reset();
    m_pShaderEASU.reset();
    m_pShaderRCAS.reset();
    m_pBufEASU.reset();
    m_pBufRCAS.reset();
    m_pTexEASU.reset();
    m_pTexRCAS.reset();
    m_pUavEASU.reset();
    m_pUavRCAS.reset();
    m_pSrvRCAS.reset();
    m_pTexFXAA.reset();
    m_pSrvFXAA.reset();
    m_pRtvFXAA.reset();
}

void CSuperResolutionMgr::CreateResources()
{
    print_debug("GTA5_FSR: Creating default resources...");

    m_bResourcesAvailable = false;

    ID3D11SamplerState* pSampler;
    if (FAILED(CreateSampler(m_pDevice, &pSampler)))
    {
        print_debug("GTA5_FSR: Failed to create FSR Sampler");
        return;
    }
    m_pSampler.attach(pSampler);

    ID3D11ComputeShader* pComputeShader;
    if (FAILED(CreateComputeShader(m_pDevice, fsr_easu_cs_d3d, &pComputeShader)))
    {
        print_debug("GTA5_FSR: Failed to create EASU shader");
        return;
    }
    m_pShaderEASU.attach(pComputeShader);

    if (FAILED(CreateComputeShader(m_pDevice, fsr_rcas_cs_d3d, &pComputeShader)))
    {
        print_debug("GTA5_FSR: Failed to create RCAS shader");
        return;
    }
    m_pShaderRCAS.attach(pComputeShader);

    UpdateSharpness();

    m_bResourcesAvailable = true;
}

bool CSuperResolutionMgr::UpdateResources(const D3D11_TEXTURE2D_DESC& src_desc, const D3D11_TEXTURE2D_DESC& dst_desc)
{
    auto src_width = static_cast<float>(src_desc.Width);
    auto src_height = static_cast<float>(src_desc.Height);

    auto dst_width = static_cast<float>(dst_desc.Width);
    auto dst_height = static_cast<float>(dst_desc.Height);

    if (m_cbEASU.cTextureSize[0] != src_width || m_cbEASU.cTextureSize[1] != src_height
        || m_cbEASU.cViewportSize[0] != dst_width || m_cbEASU.cViewportSize[1] != dst_height
        || m_pBufEASU == nullptr || m_pTexEASU == nullptr || m_pTexRCAS == nullptr
        || m_pUavEASU == nullptr || m_pUavRCAS == nullptr || m_pSrvRCAS == nullptr)
    {
        print_debug("GTA5_FSR: Updating resources... Src: [%dx%d]->[%dx%d], Dst: [%dx%d]->[%dx%d]"
            , static_cast<int>(m_cbEASU.cTextureSize[0]), static_cast<int>(m_cbEASU.cTextureSize[1]), src_desc.Width, src_desc.Height
            , static_cast<int>(m_cbEASU.cViewportSize[0]), static_cast<int>(m_cbEASU.cViewportSize[1]), dst_desc.Width, dst_desc.Height);

        m_cbEASU.cTextureSize = { src_width, src_height, 1.f, 1.f };
        m_cbEASU.cViewportSize = { dst_width, dst_height, 0.f, 0.f };

        ID3D11Buffer* pBuf;
        if (FAILED(CreateConstantBuffer(m_pDevice, &m_cbEASU, &pBuf)))
        {
            print_debug("GTA5_FSR: Failed to create EASU cbuffer");
            return false;
        }
        m_pBufEASU.attach(pBuf);

        ID3D11Texture2D* pTexture;
        if (FAILED(CreateDestinationTexture(m_pDevice, dst_desc, &pTexture)))
        {
            print_debug("GTA5_FSR: Failed to create EASU texture");
            return false;
        }
        m_pTexEASU.attach(pTexture);

        if (FAILED(CreateDestinationTexture(m_pDevice, dst_desc, &pTexture)))
        {
            print_debug("GTA5_FSR: Failed to create RCAS texture");
            return false;
        }
        m_pTexRCAS.attach(pTexture);

        ID3D11UnorderedAccessView* pUav;
        if (FAILED(CreateTextureUAV(m_pDevice, m_pTexEASU.get(), &pUav)))
        {
            print_debug("GTA5_FSR: Failed to create EASU UAV");
            return false;
        }
        m_pUavEASU.attach(pUav);

        if (FAILED(CreateTextureUAV(m_pDevice, m_pTexRCAS.get(), &pUav)))
        {
            print_debug("GTA5_FSR: Failed to create RCAS UAV");
            return false;
        }
        m_pUavRCAS.attach(pUav);

        ID3D11ShaderResourceView* pSrv;
        if (FAILED(CreateTextureSRV(m_pDevice, m_pTexEASU.get(), &pSrv)))
        {
            print_debug("GTA5_FSR: Failed to create RCAS SRV");
            return false;
        }
        m_pSrvRCAS.attach(pSrv);

        m_ThreadGroupsXY = GetThreadGroupsXY(dst_desc.Width, dst_desc.Height);

        D3D11_TEXTURE2D_DESC fxaa_tex_desc = dst_desc;
        fxaa_tex_desc.Width = src_desc.Width;
        fxaa_tex_desc.Height = src_desc.Height;

        if (FAILED(m_pDevice->CreateTexture2D(&fxaa_tex_desc, nullptr, &pTexture)))
        {
            print_debug("GTA5_FSR: Failed to create FXAA texture");
            return false;
        }
        m_pTexFXAA.attach(pTexture);

        ID3D11RenderTargetView* pRtv;
        if (FAILED(CreateTextureRTV(m_pDevice, m_pTexFXAA.get(), &pRtv)))
        {
            print_debug("GTA5_FSR: Failed to create FXAA RTV");
            return false;
        }
        m_pRtvFXAA.attach(pRtv);

        if (FAILED(CreateTextureSRV(m_pDevice, m_pTexFXAA.get(), &pSrv)))
        {
            print_debug("GTA5_FSR: Failed to create FXAA SRV");
            return false;
        }
        m_pSrvFXAA.attach(pSrv);

        UpdateAutoSharpness(src_width / dst_width);

        print_debug("GTA5_FSR: Updated resources.");
    };

    return true;
}

std::pair<UINT, UINT> CSuperResolutionMgr::GetThreadGroupsXY(UINT width, UINT height)
{
    std::pair<UINT, UINT> result;

    static constexpr int threads = 16;
    result.first = (width + (threads - 1)) / threads;
    result.second = (height + (threads - 1)) / threads;

    return result;
}

void CSuperResolutionMgr::HandleInputKeys()
{
    if (g_Cfg.KeyToggleFSR.has_value() && (GetAsyncKeyState(g_Cfg.KeyToggleFSR.value()) & 0x1))
    {
        g_Cfg.UseFidelityFxSuperResolution ^= true;
        g_Cfg.UseFidelityFxSuperResolution ? print_debug("GTA5_FSR: ON") : print_debug("GTA5_FSR: OFF");
    }
    else if (g_Cfg.KeyUpdateSharpness.has_value() && (GetAsyncKeyState(g_Cfg.KeyUpdateSharpness.value()) & 0x1))
    {
        CSimpleIniA ini;
        if (ini.LoadFile(GTA5_FSR_CFG_FILENAME) == SI_OK)
        {
            float sharpness;
            if (ini_read_numeric(ini, "GLOBAL", GTA5_FSR_CFG_SHARPNESS, sharpness) && sharpness >= 0.f && sharpness <= 1.f)
                m_fSharpness = sharpness;
        }

        UpdateSharpness();
    }
}

void CSuperResolutionMgr::UpdateSharpness()
{
    if (m_fSharpness != m_cbRCAS.cSharpness)
    {
        m_cbRCAS.cSharpness = 1.f - m_fSharpness;

        ID3D11Buffer* pBuf;
        if (FAILED(CreateConstantBuffer(m_pDevice, &m_cbRCAS, &pBuf)))
        {
            print_debug("GTA5_FSR: Failed to create RCAS cbuffer");
            return;
        }
        m_pBufRCAS.attach(pBuf);

        print_debug("GTA5_FSR: New sharpness: %f", m_fSharpness);
    }
}

void CSuperResolutionMgr::UpdateAutoSharpness(float ratio)
{
    if (g_Cfg.AutoSharpness)
    {
        // Performance: 0.5x = 0.7
        // Balanced: 0.667x = 0.5
        // Quality: 0.75x = 0.3
        // UltraQuality 0.883x = 0.0
        static std::map<float, float> ratio2sharpness = { {1.f / 2.f, 0.7f}, {2.f / 3.f, 0.5f}, {3.f / 4.f, 0.3f}, {5.f / 6.f, 0.0f} };

        auto it = std::find_if(ratio2sharpness.begin(), ratio2sharpness.end(), [ratio](const auto& p) {return std::abs(p.first - ratio) < GTA5_FSR_EPSILON; });
        if (it != ratio2sharpness.end())
        {
            m_fSharpness = it->second;
        }

        UpdateSharpness();
    }
}

void CSuperResolutionMgr::DetectMSAA()
{
    if (m_bDetectMSAA)
    {
        wil::com_ptr_t<ID3D11Buffer> pBuf;
        m_pDeviceContext->PSGetConstantBuffers(GTA5_CB_SLOT_MAIN_GLOBALS, 1, pBuf.addressof());
        auto data = ReadConstantBuffer<gta5_main_globals_cb_t>(m_pDevice, m_pDeviceContext, pBuf.get());
        if (data)
        {
            switch (data->msaa)
            {
            case 0:
            case 2:
            case 4:
            case 8:
                m_MSAA = data->msaa;
                break;
            default:
                m_MSAA = 0;
            }
        }

        m_bDetectMSAA = false;
    }
}
