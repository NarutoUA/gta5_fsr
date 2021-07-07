#pragma once

#include <dxgi1_2.h>

class IDXGIFactoryWrapper : public IDXGIFactory2
{
public:
    IDXGIFactoryWrapper(IDXGIFactory* pOrig);
    IDXGIFactoryWrapper(IDXGIFactory1* pOrig1);
    IDXGIFactoryWrapper(IDXGIFactory2* pOrig2);

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
    virtual HRESULT __stdcall GetParent(REFIID riid, void** ppParent) override
    {
        return m_pOrig->GetParent(riid, ppParent);
    }
    virtual HRESULT __stdcall EnumAdapters(UINT Adapter, IDXGIAdapter** ppAdapter) override
    {
        return m_pOrig->EnumAdapters(Adapter, ppAdapter);
    }
    virtual HRESULT __stdcall MakeWindowAssociation(HWND WindowHandle, UINT Flags) override
    {
        return m_pOrig->MakeWindowAssociation(WindowHandle, Flags);
    }
    virtual HRESULT __stdcall GetWindowAssociation(HWND* pWindowHandle) override
    {
        return m_pOrig->GetWindowAssociation(pWindowHandle);
    }

    virtual HRESULT __stdcall CreateSwapChain(IUnknown* pDevice, DXGI_SWAP_CHAIN_DESC* pDesc, IDXGISwapChain** ppSwapChain) override
    {
        return m_pOrig->CreateSwapChain(pDevice, pDesc, ppSwapChain);
    }

    virtual HRESULT __stdcall CreateSoftwareAdapter(HMODULE Module, IDXGIAdapter** ppAdapter) override
    {
        return m_pOrig->CreateSoftwareAdapter(Module, ppAdapter);
    }

    // IDXGIAdapter1
    virtual HRESULT __stdcall EnumAdapters1(UINT Adapter, IDXGIAdapter1** ppAdapter) override
    {
        return m_pOrig1->EnumAdapters1(Adapter, ppAdapter);
    }
    virtual BOOL __stdcall IsCurrent(void) override
    {
        return m_pOrig1->IsCurrent();
    }

    // IDXGIAdapter2
    virtual BOOL __stdcall IsWindowedStereoEnabled(void) override
    {
        return m_pOrig2->IsWindowedStereoEnabled();
    }

    virtual HRESULT __stdcall CreateSwapChainForHwnd(IUnknown* pDevice, HWND hWnd, const DXGI_SWAP_CHAIN_DESC1* pDesc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) override;

    virtual HRESULT __stdcall CreateSwapChainForCoreWindow(IUnknown* pDevice, IUnknown* pWindow, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain) override
    {
        return m_pOrig2->CreateSwapChainForCoreWindow(pDevice, pWindow, pDesc, pRestrictToOutput, ppSwapChain);
    }
    virtual HRESULT __stdcall GetSharedResourceAdapterLuid(HANDLE hResource, LUID* pLuid) override
    {
        return m_pOrig2->GetSharedResourceAdapterLuid(hResource, pLuid);
    }
    virtual HRESULT __stdcall RegisterStereoStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie) override
    {
        return m_pOrig2->RegisterStereoStatusWindow(WindowHandle, wMsg, pdwCookie);
    }
    virtual HRESULT __stdcall RegisterStereoStatusEvent(HANDLE hEvent, DWORD* pdwCookie) override
    {
        return m_pOrig2->RegisterStereoStatusEvent(hEvent, pdwCookie);
    }
    virtual void __stdcall UnregisterStereoStatus(DWORD dwCookie) override
    {
        return m_pOrig2->UnregisterStereoStatus(dwCookie);
    }
    virtual HRESULT __stdcall RegisterOcclusionStatusWindow(HWND WindowHandle, UINT wMsg, DWORD* pdwCookie) override
    {
        return m_pOrig2->RegisterOcclusionStatusWindow(WindowHandle, wMsg, pdwCookie);
    }
    virtual HRESULT __stdcall RegisterOcclusionStatusEvent(HANDLE hEvent, DWORD* pdwCookie) override
    {
        return m_pOrig2->RegisterOcclusionStatusEvent(hEvent, pdwCookie);
    }
    virtual void __stdcall UnregisterOcclusionStatus(DWORD dwCookie) override
    {
        return m_pOrig2->UnregisterOcclusionStatus(dwCookie);
    }
    virtual HRESULT __stdcall CreateSwapChainForComposition(IUnknown* pDevice, const DXGI_SWAP_CHAIN_DESC1* pDesc, IDXGIOutput* pRestrictToOutput, IDXGISwapChain1** ppSwapChain);

private:
    IUnknown* m_pMainOrig;
    wil::com_ptr_t<IDXGIFactory> m_pOrig;
    wil::com_ptr_t<IDXGIFactory1> m_pOrig1;
    wil::com_ptr_t<IDXGIFactory2> m_pOrig2;
    long m_RefCount;
};
