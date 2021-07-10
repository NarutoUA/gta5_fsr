#pragma once

#include "shaders/defines.h"

inline constexpr auto GTA5_FSR_DEFAULT_SHARPNESS = 0.5f;

class CSuperResolutionMgr
{
public:
    static void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    static bool RunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView> src, wil::com_ptr_t<ID3D11RenderTargetView> dst, bool fxaa);
    static void OnDeviceLost();

    static void UpdateMSAA() { instance().m_bDetectMSAA = true; }

    ~CSuperResolutionMgr();

private:
    CSuperResolutionMgr() = default;
    CSuperResolutionMgr(const CSuperResolutionMgr&) = delete;
    CSuperResolutionMgr& operator=(const CSuperResolutionMgr&) = delete;

    static CSuperResolutionMgr& instance();

    void InternalInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    bool InternalRunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView>&& src, wil::com_ptr_t<ID3D11RenderTargetView>&& dst, bool fxaa);
    void InternalOnDeviceLost();

    void ReleaseResources();
    void CreateResources();
    bool UpdateResources(const D3D11_TEXTURE2D_DESC& src_desc, const D3D11_TEXTURE2D_DESC& dst_desc);
    void HandleInputKeys();
    void UpdateSharpness();
    void UpdateAutoSharpness(float ratio);
    void DetectMSAA();

    std::pair<UINT, UINT> GetThreadGroupsXY(UINT width, UINT height);

private:
    float m_fSharpness = GTA5_FSR_DEFAULT_SHARPNESS;

    bool m_bResourcesAvailable = false;
    ID3D11Device* m_pDevice = nullptr;
    ID3D11DeviceContext* m_pDeviceContext = nullptr;

    wil::com_ptr_t<ID3D11SamplerState> m_pSampler;
    wil::com_ptr_t<ID3D11ComputeShader> m_pShaderEASU;
    wil::com_ptr_t<ID3D11ComputeShader> m_pShaderRCAS;
    wil::com_ptr_t<ID3D11Buffer> m_pBufEASU;
    wil::com_ptr_t<ID3D11Buffer> m_pBufRCAS;
    wil::com_ptr_t<ID3D11Texture2D> m_pTexEASU;
    wil::com_ptr_t<ID3D11Texture2D> m_pTexRCAS;
    wil::com_ptr_t<ID3D11UnorderedAccessView> m_pUavEASU;
    wil::com_ptr_t<ID3D11UnorderedAccessView> m_pUavRCAS;
    wil::com_ptr_t<ID3D11ShaderResourceView> m_pSrvRCAS;
    wil::com_ptr_t<ID3D11Texture2D> m_pTexFXAA;
    wil::com_ptr_t<ID3D11ShaderResourceView> m_pSrvFXAA;
    wil::com_ptr_t<ID3D11RenderTargetView> m_pRtvFXAA;

    cb_easu_t m_cbEASU;
    cb_rcas_t m_cbRCAS;

    UINT m_MSAA = 0;
    bool m_bDetectMSAA = false;

    std::pair<UINT, UINT> m_ThreadGroupsXY;
};
