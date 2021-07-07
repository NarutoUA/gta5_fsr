#pragma once

#include "IDXGIAdapterWrapper.h"

class IDXGIDeviceWrapper : public IDXGIDevice
{
public:
    IDXGIDeviceWrapper(IDXGIDevice* pOrig);

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return m_pOrig->QueryInterface(riid, ppvObject);
    }

    virtual ULONG __stdcall AddRef(void) override;
    virtual ULONG __stdcall Release(void) override;

    virtual HRESULT __stdcall SetPrivateData(REFGUID Name, UINT DataSize, const void* pData) override
    {
        return m_pOrig->SetPrivateData(Name, DataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateDataInterface(REFGUID Name, const IUnknown* pUnknown) override
    {
        return m_pOrig->SetPrivateDataInterface(Name, pUnknown);
    }
    virtual HRESULT __stdcall GetPrivateData(REFGUID Name, UINT* pDataSize, void* pData) override
    {
        return m_pOrig->GetPrivateData(Name, pDataSize, pData);
    }

    virtual HRESULT __stdcall GetParent(REFIID riid, void** ppParent) override
    {
        return m_pOrig->GetParent(riid, ppParent);
    }

    virtual HRESULT __stdcall GetAdapter(IDXGIAdapter** pAdapter) override;

    virtual HRESULT __stdcall CreateSurface(const DXGI_SURFACE_DESC* pDesc, UINT NumSurfaces, DXGI_USAGE Usage, const DXGI_SHARED_RESOURCE* pSharedResource, IDXGISurface** ppSurface) override
    {
        return m_pOrig->CreateSurface(pDesc, NumSurfaces, Usage, pSharedResource, ppSurface);
    }
    virtual HRESULT __stdcall QueryResourceResidency(IUnknown* const* ppResources, DXGI_RESIDENCY* pResidencyStatus, UINT NumResources) override
    {
        return m_pOrig->QueryResourceResidency(ppResources, pResidencyStatus, NumResources);
    }
    virtual HRESULT __stdcall SetGPUThreadPriority(INT Priority) override
    {
        return m_pOrig->SetGPUThreadPriority(Priority);
    }
    virtual HRESULT __stdcall GetGPUThreadPriority(INT* pPriority) override
    {
        return m_pOrig->GetGPUThreadPriority(pPriority);
    }

private:
    IDXGIDevice* m_pOrig;
    long m_RefCount;
};
