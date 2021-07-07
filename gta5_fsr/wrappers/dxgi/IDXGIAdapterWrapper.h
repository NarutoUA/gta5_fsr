#pragma once

class IDXGIAdapterWrapper : public IDXGIAdapter
{
public:
    IDXGIAdapterWrapper(IDXGIAdapter* pOrig);

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override;

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

    virtual HRESULT __stdcall GetParent(REFIID riid, void** ppParent) override;

    virtual HRESULT __stdcall EnumOutputs(UINT Output, IDXGIOutput** ppOutput) override
    {
        return m_pOrig->EnumOutputs(Output, ppOutput);
    }
    virtual HRESULT __stdcall GetDesc(DXGI_ADAPTER_DESC* pDesc) override
    {
        return m_pOrig->GetDesc(pDesc);
    }
    virtual HRESULT __stdcall CheckInterfaceSupport(REFGUID InterfaceName, LARGE_INTEGER* pUMDVersion) override
    {
        return m_pOrig->CheckInterfaceSupport(InterfaceName, pUMDVersion);
    }

private:
    IDXGIAdapter* m_pOrig;
    long m_RefCount;
};
