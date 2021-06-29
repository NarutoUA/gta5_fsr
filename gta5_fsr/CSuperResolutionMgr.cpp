#include "stdafx.h"
#include <numeric>
#include <charconv>

#include "CSuperResolutionMgr.h"

#include "simpleini/SimpleIni.h"
#include "helpers/d3d11_helper.h"
#include "helpers/ini_helper.h"
#include "shaders/fsr_easu_cs_d3d.h"
#include "shaders/fsr_rcas_cs_d3d.h"

constexpr auto GTA5_FSR_CFG_FILENAME = "gta5_fsr.ini";
constexpr auto GTA5_FSR_CFG_USE_FSR = "UseFidelityFxSuperResolution";
constexpr auto GTA5_FSR_CFG_AUTO_SHARPNESS = "AutoSharpness";
constexpr auto GTA5_FSR_CFG_SHARPNESS = "Sharpness";
constexpr auto GTA5_FSR_CFG_KEY_TOGGLE_FSR = "KeyToggleFSR";
constexpr auto GTA5_FSR_CFG_KEY_UPDATE_SHARPNESS = "KeyUpdateSharpness";
constexpr auto GTA5_FSR_EPSILON = 0.001;

CSuperResolutionMgr& CSuperResolutionMgr::instance()
{
    static CSuperResolutionMgr instance;
    return instance;
}

void CSuperResolutionMgr::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    instance().InternalInitialize(pDevice, pDeviceContext);
}

bool CSuperResolutionMgr::RunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView> src, wil::com_ptr_t<ID3D11RenderTargetView> dst)
{
    return instance().InternalRunSuperResolutionPass(std::move(src), std::move(dst));
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

    ReadConfig();
    CreateResources();

    OutputDebugStringA("GTA5_FSR: Initialized");
}

bool CSuperResolutionMgr::InternalRunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView>&& src, wil::com_ptr_t<ID3D11RenderTargetView>&& dst)
{
    HandleInputKeys();

    if (m_bUseSuperResolution == false || m_bResourcesAvailable == false || src == nullptr || dst == nullptr)
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

    if (src_tex_desc.Width >= dst_tex_desc.Width || src_tex_desc.Height >= dst_tex_desc.Height
        || std::abs(src_ratio - dst_ratio) > GTA5_FSR_EPSILON)
        return false;

    // Run FidelityFx Super Resolution pass
    if (UpdateResources(src_tex_desc, dst_tex_desc) == false)
        return false;

    if (FAILED(RunComputeShader(m_pDeviceContext, m_pShaderEASU.get(), src.get(), m_pBufEASU.get(), m_pUavEASU.get(), m_pSampler.get(), m_ThreadGroupsXY.first, m_ThreadGroupsXY.second, 1)))
        return false;

    if (FAILED(RunComputeShader(m_pDeviceContext, m_pShaderRCAS.get(), m_pSrvRCAS.get(), m_pBufRCAS.get(), m_pUavRCAS.get(), nullptr, m_ThreadGroupsXY.first, m_ThreadGroupsXY.second, 1)))
        return false;

    m_pDeviceContext->CopyResource(dst_tex.get(), m_pTexRCAS.get());
    return true;
}

void CSuperResolutionMgr::InternalOnDeviceLost()
{
    ReleaseResources();
}

void CSuperResolutionMgr::ReadConfig()
{
    CSimpleIniA ini;
    if (ini.LoadFile(GTA5_FSR_CFG_FILENAME) == SI_OK)
    {
        OutputDebugStringA("GTA5_FSR: Reading config file...");

        bool use_fsr;
        if (ini_read_bool(ini, "GLOBAL", GTA5_FSR_CFG_USE_FSR, use_fsr))
            m_bUseSuperResolution = use_fsr;

        bool auto_sharpness;
        if (ini_read_bool(ini, "GLOBAL", GTA5_FSR_CFG_AUTO_SHARPNESS, auto_sharpness))
            m_bAutoSharpness = auto_sharpness;
        
        float sharpness;
        if (ini_read_numeric(ini, "GLOBAL", GTA5_FSR_CFG_SHARPNESS, sharpness) && sharpness >= 0.f && sharpness <= 1.f)
            m_fSharpness = sharpness;

        int code;
        if (ini_read_numeric(ini, "INPUT", GTA5_FSR_CFG_KEY_TOGGLE_FSR, code))
            m_keyToggleFSR = code;

        if (ini_read_numeric(ini, "INPUT", GTA5_FSR_CFG_KEY_UPDATE_SHARPNESS, code))
            m_keyUpdateSharpness = code;
    }
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
}

void CSuperResolutionMgr::CreateResources()
{
    OutputDebugStringA("GTA5_FSR: Creating default resources...");

    m_bResourcesAvailable = false;

    ID3D11SamplerState* pSampler;
    if (FAILED(CreateSampler(m_pDevice, &pSampler)))
        return;
    m_pSampler.attach(pSampler);

    ID3D11ComputeShader* pComputeShader;
    if (FAILED(CreateComputeShader(m_pDevice, fsr_easu_cs_d3d, &pComputeShader)))
        return;

    m_pShaderEASU.attach(pComputeShader);

    if (FAILED(CreateComputeShader(m_pDevice, fsr_rcas_cs_d3d, &pComputeShader)))
        return;
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
        m_cbEASU.cTextureSize = { src_width, src_height, 1.f, 1.f };
        m_cbEASU.cViewportSize = { dst_width, dst_height, 0.f, 0.f };

        ID3D11Buffer* pBuf;
        if (FAILED(CreateConstantBuffer(m_pDevice, &m_cbEASU, &pBuf)))
            return false;
        m_pBufEASU.attach(pBuf);

        ID3D11Texture2D* pTexture;
        if (FAILED(CreateDestinationTexture(m_pDevice, dst_desc, &pTexture)))
            return false;
        m_pTexEASU.attach(pTexture);

        if (FAILED(CreateDestinationTexture(m_pDevice, dst_desc, &pTexture)))
            return false;
        m_pTexRCAS.attach(pTexture);

        ID3D11UnorderedAccessView* pUav;
        if (FAILED(CreateTextureUAV(m_pDevice, m_pTexEASU.get(), &pUav)))
            return false;
        m_pUavEASU.attach(pUav);

        if (FAILED(CreateTextureUAV(m_pDevice, m_pTexRCAS.get(), &pUav)))
            return false;
        m_pUavRCAS.attach(pUav);

        ID3D11ShaderResourceView* pSrv;
        if (FAILED(CreateTextureSRV(m_pDevice, m_pTexEASU.get(), &pSrv)))
            return false;
        m_pSrvRCAS.attach(pSrv);

        m_ThreadGroupsXY = GetThreadGroupsXY(dst_desc.Width, dst_desc.Height);

        UpdateAutoSharpness(src_width / dst_width);

        OutputDebugStringA("GTA5_FSR: Updated some resources.");
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
    if (m_keyToggleFSR.has_value() && (GetAsyncKeyState(m_keyToggleFSR.value()) & 0x1))
    {
        m_bUseSuperResolution ^= true;
        m_bUseSuperResolution ? OutputDebugStringA("GTA5_FSR: ON") : OutputDebugStringA("GTA5_FSR: OFF");
    }
    else if (m_keyUpdateSharpness.has_value() && (GetAsyncKeyState(m_keyUpdateSharpness.value()) & 0x1))
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
        if (SUCCEEDED(CreateConstantBuffer(m_pDevice, &m_cbRCAS, &pBuf)))
            m_pBufRCAS.attach(pBuf);

        char buf[64] = { 0 };
        sprintf_s(buf, "GTA5_FSR: New sharpness: %f", m_fSharpness);
        OutputDebugStringA(buf);
    }
}

void CSuperResolutionMgr::UpdateAutoSharpness(float ratio)
{
    if (m_bAutoSharpness)
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
