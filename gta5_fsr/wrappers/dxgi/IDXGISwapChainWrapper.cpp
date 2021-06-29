#include "stdafx.h"
#include "IDXGISwapChainWrapper.h"

IDXGISwapChainWrapper::IDXGISwapChainWrapper(IDXGISwapChain* pOrigSwapChain)
    : m_pOrigSwapChain(pOrigSwapChain)
{
}

ULONG __stdcall IDXGISwapChainWrapper::Release(void)
{
    auto count = m_pOrigSwapChain->Release();

    if (count == 0)
        delete this;

    return count;
}

HRESULT __stdcall IDXGISwapChainWrapper::Present(UINT SyncInterval, UINT Flags)
{
    auto hr = m_pOrigSwapChain->Present(SyncInterval, Flags);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        CSuperResolutionMgr::OnDeviceLost();
    }
    return hr;
}
