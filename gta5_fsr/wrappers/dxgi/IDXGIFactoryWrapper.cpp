#include "stdafx.h"
#include "IDXGIFactoryWrapper.h"

IDXGIFactoryWrapper::IDXGIFactoryWrapper(IDXGIFactory* pOrig)
    : m_RefCount(1)
{
    pOrig->AddRef();
    m_pOrig.attach(pOrig);
    if (SUCCEEDED(pOrig->QueryInterface(m_pOrig1.addressof())))
        m_pOrig1->QueryInterface(m_pOrig2.addressof());

    m_pMainOrig = pOrig;
}

IDXGIFactoryWrapper::IDXGIFactoryWrapper(IDXGIFactory1* pOrig1)
    : m_RefCount(1)
{
    pOrig1->AddRef();
    m_pOrig1.attach(pOrig1);
    if (SUCCEEDED(pOrig1->QueryInterface(m_pOrig.addressof())))
        m_pOrig->QueryInterface(m_pOrig2.addressof());

    m_pMainOrig = pOrig1;
}

IDXGIFactoryWrapper::IDXGIFactoryWrapper(IDXGIFactory2* pOrig2)
    : m_RefCount(1)
{
    pOrig2->AddRef();
    m_pOrig2.attach(pOrig2);
    if (SUCCEEDED(pOrig2->QueryInterface(m_pOrig.addressof())))
        m_pOrig->QueryInterface(m_pOrig1.addressof());

    m_pMainOrig = pOrig2;
}

HRESULT __stdcall IDXGIFactoryWrapper::QueryInterface(REFIID riid, void** ppvObject)
{
    if ((riid == IID_IDXGIFactory && m_pOrig)
        || (riid == IID_IDXGIFactory1 && m_pOrig1)
        || (riid == IID_IDXGIFactory2 && m_pOrig2))
    {
        AddRef();
        *ppvObject = this;
        return S_OK;
    }
    return m_pMainOrig->QueryInterface(riid, ppvObject);
}

ULONG __stdcall IDXGIFactoryWrapper::AddRef(void)
{
    InterlockedIncrement(&m_RefCount);
    return m_pMainOrig->AddRef();
}

ULONG __stdcall IDXGIFactoryWrapper::Release(void)
{
    auto refCountOrig = m_pMainOrig->Release();
    auto refCountMy = InterlockedDecrement(&m_RefCount);

    if (refCountOrig == 0 || refCountMy == 0)
        delete this;

    return refCountOrig;
}

HRESULT __stdcall IDXGIFactoryWrapper::CreateSwapChainForHwnd(IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
    OutputDebugStringA("GTA5_FSR: Wrap IDXGISwapChain1 [hwnd-fivem]");

    auto hr = m_pOrig2->CreateSwapChainForHwnd(pDevice, hWnd, pDesc, pFullscreenDesc, pRestrictToOutput, ppSwapChain);
    if (SUCCEEDED(hr) && ppSwapChain && *ppSwapChain)
    {
        OutputDebugStringA("GTA5_FSR: Wrapping IDXGISwapChain1 [hwnd-fivem]");
        *ppSwapChain = new IDXGISwapChainWrapper(*ppSwapChain);
    }
    return hr;
}

HRESULT __stdcall IDXGIFactoryWrapper::CreateSwapChainForComposition(IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain)
{
    OutputDebugStringA("GTA5_FSR: Wrap IDXGISwapChain1 [composition-fivem]");

    auto hr = m_pOrig2->CreateSwapChainForComposition(pDevice, pDesc, pRestrictToOutput, ppSwapChain);
    if (SUCCEEDED(hr) && ppSwapChain && *ppSwapChain)
    {
        OutputDebugStringA("GTA5_FSR: Wrapping IDXGISwapChain1 [composition-fivem]");
        *ppSwapChain = new IDXGISwapChainWrapper(*ppSwapChain);
    }
    return hr;
}
