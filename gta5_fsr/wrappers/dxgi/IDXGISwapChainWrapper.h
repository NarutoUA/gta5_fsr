#pragma once

class IDXGISwapChainWrapper : public IDXGISwapChain
{
public:
    IDXGISwapChainWrapper(IDXGISwapChain* pOrig);

    IDXGISwapChainWrapper(const IDXGISwapChainWrapper&) = delete;
    IDXGISwapChainWrapper& operator=(const IDXGISwapChainWrapper) = delete;

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return m_pOrig->QueryInterface(riid, ppvObject);
    }
    virtual ULONG __stdcall AddRef(void) override
    {
        return m_pOrig->AddRef();
    }

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
    virtual HRESULT __stdcall GetDevice(REFIID riid, void** ppDevice) override
    {
        return m_pOrig->GetDevice(riid, ppDevice);
    }

    virtual HRESULT __stdcall Present(UINT SyncInterval, UINT Flags) override;

    virtual HRESULT __stdcall GetBuffer(UINT Buffer, REFIID riid, void** ppSurface) override
    {
        return m_pOrig->GetBuffer(Buffer, riid, ppSurface);
    }
    virtual HRESULT __stdcall SetFullscreenState(BOOL Fullscreen, IDXGIOutput* pTarget) override
    {
        return m_pOrig->SetFullscreenState(Fullscreen, pTarget);
    }
    virtual HRESULT __stdcall GetFullscreenState(BOOL* pFullscreen, IDXGIOutput** ppTarget) override
    {
        return m_pOrig->GetFullscreenState(pFullscreen, ppTarget);
    }
    virtual HRESULT __stdcall GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc) override
    {
        return m_pOrig->GetDesc(pDesc);
    }
    virtual HRESULT __stdcall ResizeBuffers(UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags) override
    {
        return m_pOrig->ResizeBuffers(BufferCount, Width, Height, NewFormat, SwapChainFlags);
    }
    virtual HRESULT __stdcall ResizeTarget(const DXGI_MODE_DESC* pNewTargetParameters) override
    {
        return m_pOrig->ResizeTarget(pNewTargetParameters);
    }
    virtual HRESULT __stdcall GetContainingOutput(IDXGIOutput** ppOutput) override
    {
        return m_pOrig->GetContainingOutput(ppOutput);
    }
    virtual HRESULT __stdcall GetFrameStatistics(DXGI_FRAME_STATISTICS* pStats) override
    {
        return m_pOrig->GetFrameStatistics(pStats);
    }
    virtual HRESULT __stdcall GetLastPresentCount(UINT* pLastPresentCount) override
    {
        return m_pOrig->GetLastPresentCount(pLastPresentCount);
    }

private:
    IDXGISwapChain* m_pOrig;
};
