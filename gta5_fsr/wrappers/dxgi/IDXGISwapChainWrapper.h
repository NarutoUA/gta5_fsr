#pragma once

class IDXGISwapChainWrapper : public IDXGISwapChain
{
public:
    IDXGISwapChainWrapper(IDXGISwapChain* pOrigSwapChain);

    IDXGISwapChainWrapper(const IDXGISwapChainWrapper&) = delete;
    IDXGISwapChainWrapper& operator=(const IDXGISwapChainWrapper) = delete;

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return m_pOrigSwapChain->QueryInterface(riid, ppvObject);
    }
    virtual ULONG __stdcall AddRef(void) override
    {
        return m_pOrigSwapChain->AddRef();
    }

    virtual ULONG __stdcall Release(void) override;

    virtual HRESULT __stdcall SetPrivateData(REFGUID Name, UINT DataSize, const void* pData) override
    {
        return m_pOrigSwapChain->SetPrivateData(Name, DataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateDataInterface(REFGUID Name, const IUnknown* pUnknown) override
    {
        return m_pOrigSwapChain->SetPrivateDataInterface(Name, pUnknown);
    }
    virtual HRESULT __stdcall GetPrivateData(REFGUID Name, UINT* pDataSize, void* pData) override
    {
        return m_pOrigSwapChain->GetPrivateData(Name, pDataSize, pData);
    }
    virtual HRESULT __stdcall GetParent(REFIID riid, void** ppParent) override
    {
        return m_pOrigSwapChain->GetParent(riid, ppParent);
    }
    virtual HRESULT __stdcall GetDevice(REFIID riid, void** ppDevice) override
    {
        return m_pOrigSwapChain->GetDevice(riid, ppDevice);
    }

    virtual HRESULT __stdcall Present(UINT SyncInterval, UINT Flags) override;

    virtual HRESULT __stdcall GetBuffer(UINT Buffer, REFIID riid, void** ppSurface) override
    {
        return m_pOrigSwapChain->GetBuffer(Buffer, riid, ppSurface);
    }
    virtual HRESULT __stdcall SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget) override
    {
        return m_pOrigSwapChain->SetFullscreenState(Fullscreen, pTarget);
    }
    virtual HRESULT __stdcall GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget) override
    {
        return m_pOrigSwapChain->GetFullscreenState(pFullscreen, ppTarget);
    }
    virtual HRESULT __stdcall GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc) override
    {
        return m_pOrigSwapChain->GetDesc(pDesc);
    }
    virtual HRESULT __stdcall ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) override
    {
        return m_pOrigSwapChain->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    }
    virtual HRESULT __stdcall ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters) override
    {
        return m_pOrigSwapChain->ResizeTarget(pNewTargetParameters);
    }
    virtual HRESULT __stdcall GetContainingOutput(IDXGIOutput** ppOutput) override
    {
        return m_pOrigSwapChain->GetContainingOutput(ppOutput);
    }
    virtual HRESULT __stdcall GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats) override
    {
        return m_pOrigSwapChain->GetFrameStatistics(pStats);
    }
    virtual HRESULT __stdcall GetLastPresentCount(UINT* pLastPresentCount) override
    {
        return m_pOrigSwapChain->GetLastPresentCount(pLastPresentCount);
    }

private:
    IDXGISwapChain* m_pOrigSwapChain;
};
