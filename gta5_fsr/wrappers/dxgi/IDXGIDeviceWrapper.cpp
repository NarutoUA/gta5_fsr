#include "stdafx.h"
#include "IDXGIDeviceWrapper.h"
#include "IDXGIAdapterWrapper.h"

IDXGIDeviceWrapper::IDXGIDeviceWrapper(IDXGIDevice* pOrig)
    : m_pOrig(pOrig)
    , m_RefCount(1)
{
}

ULONG __stdcall IDXGIDeviceWrapper::AddRef(void)
{
    InterlockedIncrement(&m_RefCount);
    return m_pOrig->AddRef();
}

ULONG __stdcall IDXGIDeviceWrapper::Release(void)
{
    auto refCountOrig = m_pOrig->Release();
    auto refCountMy = InterlockedDecrement(&m_RefCount);

    if (refCountOrig == 0 || refCountMy == 0)
        delete this;

    return refCountOrig;
}

HRESULT __stdcall IDXGIDeviceWrapper::GetAdapter(IDXGIAdapter** pAdapter)
{
    auto hr = m_pOrig->GetAdapter(pAdapter);
    if (SUCCEEDED(hr))
    {
        print_debug("GTA5_FSR: Wrapping IDXGIAdapter [fivem]");
        *pAdapter = new IDXGIAdapterWrapper(*pAdapter);
    }
    return hr;
}
