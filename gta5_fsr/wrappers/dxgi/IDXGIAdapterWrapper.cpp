#include "stdafx.h"
#include "IDXGIAdapterWrapper.h"
#include "IDXGIFactoryWrapper.h"

IDXGIAdapterWrapper::IDXGIAdapterWrapper(IDXGIAdapter* pOrig)
    : m_pOrig(pOrig)
    , m_RefCount(1)
{
}

HRESULT __stdcall IDXGIAdapterWrapper::QueryInterface(REFIID riid, void** ppvObject)
{
    if (riid == IID_IDXGIFactory)
    {
        auto hr = m_pOrig->QueryInterface(riid, ppvObject);
        if (SUCCEEDED(hr) && ppvObject && *ppvObject)
        {
            OutputDebugStringA("GTA5_FSR: Wrapping IDXGIFactory [query]");
            *ppvObject = new IDXGIFactoryWrapper(reinterpret_cast<IDXGIFactory*>(*ppvObject));
        }
        return hr;
    }

    return m_pOrig->QueryInterface(riid, ppvObject);
}

ULONG __stdcall IDXGIAdapterWrapper::AddRef(void)
{
    InterlockedIncrement(&m_RefCount);
    return m_pOrig->AddRef();
}

ULONG __stdcall IDXGIAdapterWrapper::Release(void)
{
    auto refCountOrig = m_pOrig->Release();
    auto refCountMy = InterlockedDecrement(&m_RefCount);

    if (refCountOrig == 0 || refCountMy == 0)
        delete this;

    return refCountOrig;
}

HRESULT __stdcall IDXGIAdapterWrapper::GetParent(REFIID riid, void** ppParent)
{
    if (riid == IID_IDXGIFactory2)
    {
        auto hr = m_pOrig->GetParent(riid, ppParent);
        if (SUCCEEDED(hr) && ppParent && *ppParent)
        {
            OutputDebugStringA("GTA5_FSR: Wrapping IDXGIFactory2 [fivem]");
            *ppParent = new IDXGIFactoryWrapper(reinterpret_cast<IDXGIFactory2*>(*ppParent));
        }
        return hr;
    }

    return m_pOrig->GetParent(riid, ppParent);
}
