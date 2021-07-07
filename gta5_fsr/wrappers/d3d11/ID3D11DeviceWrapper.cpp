#include "stdafx.h"

#include "ID3D11DeviceWrapper.h"
#include "../dxgi/IDXGIDeviceWrapper.h"

extern bool g_bWrapFiveMSwapChain;

ID3D11DeviceWrapper::ID3D11DeviceWrapper(ID3D11Device* pOrigDevice)
    : m_pOrig(pOrigDevice)
{
}

HRESULT __stdcall ID3D11DeviceWrapper::QueryInterface(REFIID riid, void** ppvObject)
{
    if (g_bWrapFiveMSwapChain)
    {
        if (riid == IID_IDXGIDevice)
        {
            auto hr = m_pOrig->QueryInterface(riid, ppvObject);
            if (SUCCEEDED(hr))
            {
                print_debug("GTA5_FSR: Wrapping IDXGIDevice [fivem]");
                *ppvObject = new IDXGIDeviceWrapper(reinterpret_cast<IDXGIDevice*>(*ppvObject));
            }
            return hr;
        }
    }
    return m_pOrig->QueryInterface(riid, ppvObject);
}

ULONG ID3D11DeviceWrapper::Release()
{
    auto count = m_pOrig->Release();

    if (count == 0)
        delete this;

    return count;
}

HRESULT __stdcall ID3D11DeviceWrapper::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
    // weird way to check if menu was opened
    CSuperResolutionMgr::UpdateMSAA();
    return m_pOrig->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}
