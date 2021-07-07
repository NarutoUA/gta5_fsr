#include "stdafx.h"
#include "IDXGISwapChainWrapper.h"

extern bool g_bWrapFiveMSwapChain;

IDXGISwapChainWrapper::IDXGISwapChainWrapper(IDXGISwapChain* pOrig)
    : m_RefCount(1)
{
    pOrig->AddRef();
    m_pOrig.attach(pOrig);
    pOrig->QueryInterface(m_pOrig1.addressof());

    m_pMainOrig = pOrig;

    g_bWrapFiveMSwapChain = false;
}

IDXGISwapChainWrapper::IDXGISwapChainWrapper(IDXGISwapChain1* pOrig1)
    : m_RefCount(1)
{
    pOrig1->AddRef();
    m_pOrig1.attach(pOrig1);
    pOrig1->QueryInterface(m_pOrig.addressof());

    m_pMainOrig = pOrig1;

    g_bWrapFiveMSwapChain = false;
}

HRESULT __stdcall IDXGISwapChainWrapper::QueryInterface(REFIID riid, void** ppvObject)
{
    if ((riid == IID_IDXGISwapChain && m_pOrig)
        || (riid == IID_IDXGISwapChain1 && m_pOrig1))
    {
        AddRef();
        *ppvObject = this;
        return S_OK;
    }
    return m_pMainOrig->QueryInterface(riid, ppvObject);
}

ULONG __stdcall IDXGISwapChainWrapper::AddRef()
{
    InterlockedIncrement(&m_RefCount);
    return m_pMainOrig->AddRef();
}

ULONG __stdcall IDXGISwapChainWrapper::Release(void)
{
    auto refCountOrig = m_pMainOrig->Release();
    auto refCountMy = InterlockedDecrement(&m_RefCount);

    if (refCountOrig == 0 || refCountMy == 0)
        delete this;

    return refCountOrig;
}

HRESULT __stdcall IDXGISwapChainWrapper::Present(UINT SyncInterval, UINT Flags)
{
    auto hr = m_pOrig->Present(SyncInterval, Flags);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        g_bWrapFiveMSwapChain = true;
        CSuperResolutionMgr::OnDeviceLost();
    }
    return hr;
}

HRESULT __stdcall IDXGISwapChainWrapper::Present1(UINT SyncInterval, UINT PresentFlags, const DXGI_PRESENT_PARAMETERS* pPresentParameters)
{
    auto hr = m_pOrig1->Present1(SyncInterval, PresentFlags, pPresentParameters);

    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        g_bWrapFiveMSwapChain = true;
        CSuperResolutionMgr::OnDeviceLost();
    }
    return hr;
}
