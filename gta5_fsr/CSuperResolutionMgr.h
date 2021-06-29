#pragma once

#include "shaders/defines.h"

constexpr auto GTA5_FSR_DEFAULT_SHARPNESS = 0.5f;

class CSuperResolutionMgr
{
public:
    static void Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    static bool RunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView> src, wil::com_ptr_t<ID3D11RenderTargetView> dst);
    static void OnDeviceLost();

    ~CSuperResolutionMgr();

private:
    CSuperResolutionMgr() = default;
    CSuperResolutionMgr(const CSuperResolutionMgr&) = delete;
    CSuperResolutionMgr& operator=(const CSuperResolutionMgr&) = delete;

    static CSuperResolutionMgr& instance();

    void InternalInitialize(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
    bool InternalRunSuperResolutionPass(wil::com_ptr_t<ID3D11ShaderResourceView>&& src, wil::com_ptr_t<ID3D11RenderTargetView>&& dst);
    void InternalOnDeviceLost();

    void ReadConfig();
    void ReleaseResources();
    void CreateResources();
    bool UpdateResources(const D3D11_TEXTURE2D_DESC& src_desc, const D3D11_TEXTURE2D_DESC& dst_desc);
    void HandleInputKeys();
    void UpdateSharpness();
    void UpdateAutoSharpness(float ratio);

    std::pair<UINT, UINT> GetThreadGroupsXY(UINT width, UINT height);

private:
    bool m_bUseSuperResolution = true;
    bool m_bAutoSharpness = true;
    float m_fSharpness = GTA5_FSR_DEFAULT_SHARPNESS;
    std::optional<int> m_keyToggleFSR;
    std::optional<int> m_keyUpdateSharpness;

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

    cb_easu_t m_cbEASU;
    cb_rcas_t m_cbRCAS;
    
    std::pair<UINT, UINT> m_ThreadGroupsXY;
};
