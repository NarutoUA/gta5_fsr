#pragma once

class ID3D11DeviceWrapper : public ID3D11Device
{
public:
    ID3D11DeviceWrapper(ID3D11Device* pOrigDevice);

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return m_pOrig->QueryInterface(riid, ppvObject);
    }

    virtual ULONG __stdcall AddRef(void) override
    {
        return m_pOrig->AddRef();
    }
    virtual ULONG __stdcall Release(void) override;

    virtual HRESULT __stdcall CreateBuffer(const D3D11_BUFFER_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Buffer** ppBuffer) override
    {
        return m_pOrig->CreateBuffer(pDesc, pInitialData, ppBuffer);
    }
    virtual HRESULT __stdcall CreateTexture1D(const D3D11_TEXTURE1D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture1D** ppTexture1D) override
    {
        return m_pOrig->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
    }
    virtual HRESULT __stdcall CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture2D** ppTexture2D) override
    {
        return m_pOrig->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
    }
    virtual HRESULT __stdcall CreateTexture3D(const D3D11_TEXTURE3D_DESC* pDesc, const D3D11_SUBRESOURCE_DATA* pInitialData, ID3D11Texture3D** ppTexture3D) override
    {
        return m_pOrig->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
    }
    virtual HRESULT __stdcall CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView) override
    {
        return m_pOrig->CreateShaderResourceView(pResource, pDesc, ppSRView);
    }
    virtual HRESULT __stdcall CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView) override
    {
        return m_pOrig->CreateUnorderedAccessView(pResource, pDesc, ppUAView);
    }
    virtual HRESULT __stdcall CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView) override
    {
        return m_pOrig->CreateRenderTargetView(pResource, pDesc, ppRTView);
    }
    virtual HRESULT __stdcall CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDepthStencilView) override
    {
        return m_pOrig->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
    }
    virtual HRESULT __stdcall CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs, UINT NumElements, const void* pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D11InputLayout** ppInputLayout) override
    {
        return m_pOrig->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
    }
    virtual HRESULT __stdcall CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader) override
    {
        return m_pOrig->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader);
    }
    virtual HRESULT __stdcall CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader) override
    {
        return m_pOrig->CreateGeometryShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppGeometryShader);
    }
    virtual HRESULT __stdcall CreateGeometryShaderWithStreamOutput(const void* pShaderBytecode, SIZE_T BytecodeLength, const D3D11_SO_DECLARATION_ENTRY* pSODeclaration, UINT NumEntries, const UINT* pBufferStrides, UINT NumStrides, UINT RasterizedStream, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppGeometryShader) override
    {
        return m_pOrig->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, pBufferStrides, NumStrides, RasterizedStream, pClassLinkage, ppGeometryShader);
    }
    virtual HRESULT __stdcall CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader) override
    {
        return m_pOrig->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader);
    }
    virtual HRESULT __stdcall CreateHullShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11HullShader** ppHullShader) override
    {
        return m_pOrig->CreateHullShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppHullShader);
    }
    virtual HRESULT __stdcall CreateDomainShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11DomainShader** ppDomainShader) override
    {
        return m_pOrig->CreateDomainShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppDomainShader);
    }
    virtual HRESULT __stdcall CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader) override
    {
        return m_pOrig->CreateComputeShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppComputeShader);
    }
    virtual HRESULT __stdcall CreateClassLinkage(ID3D11ClassLinkage** ppLinkage) override
    {
        return m_pOrig->CreateClassLinkage(ppLinkage);
    }
    virtual HRESULT __stdcall CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState) override
    {
        return m_pOrig->CreateBlendState(pBlendStateDesc, ppBlendState);
    }
    virtual HRESULT __stdcall CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState) override
    {
        return m_pOrig->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
    }
    virtual HRESULT __stdcall CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState) override
    {
        return m_pOrig->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
    }
    virtual HRESULT __stdcall CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState) override
    {
        return m_pOrig->CreateSamplerState(pSamplerDesc, ppSamplerState);
    }
    virtual HRESULT __stdcall CreateQuery(const D3D11_QUERY_DESC* pQueryDesc, ID3D11Query** ppQuery) override
    {
        return m_pOrig->CreateQuery(pQueryDesc, ppQuery);
    }
    virtual HRESULT __stdcall CreatePredicate(const D3D11_QUERY_DESC* pPredicateDesc, ID3D11Predicate** ppPredicate) override
    {
        return m_pOrig->CreatePredicate(pPredicateDesc, ppPredicate);
    }
    virtual HRESULT __stdcall CreateCounter(const D3D11_COUNTER_DESC* pCounterDesc, ID3D11Counter** ppCounter) override
    {
        return m_pOrig->CreateCounter(pCounterDesc, ppCounter);
    }
    virtual HRESULT __stdcall CreateDeferredContext(UINT ContextFlags, ID3D11DeviceContext** ppDeferredContext) override
    {
        return m_pOrig->CreateDeferredContext(ContextFlags, ppDeferredContext);
    }
    virtual HRESULT __stdcall OpenSharedResource(HANDLE hResource, REFIID ReturnedInterface, void** ppResource) override
    {
        return m_pOrig->OpenSharedResource(hResource, ReturnedInterface, ppResource);
    }
    virtual HRESULT __stdcall CheckFormatSupport(DXGI_FORMAT Format, UINT* pFormatSupport) override
    {
        return m_pOrig->CheckFormatSupport(Format, pFormatSupport);
    }

    virtual HRESULT __stdcall CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels) override;

    virtual void __stdcall CheckCounterInfo(D3D11_COUNTER_INFO* pCounterInfo) override
    {
        return m_pOrig->CheckCounterInfo(pCounterInfo);
    }
    virtual HRESULT __stdcall CheckCounter(const D3D11_COUNTER_DESC* pDesc, D3D11_COUNTER_TYPE* pType, UINT* pActiveCounters, LPSTR szName, UINT* pNameLength, LPSTR szUnits, UINT* pUnitsLength, LPSTR szDescription, UINT* pDescriptionLength) override
    {
        return m_pOrig->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
    }
    virtual HRESULT __stdcall CheckFeatureSupport(D3D11_FEATURE Feature, void* pFeatureSupportData, UINT FeatureSupportDataSize) override
    {
        return m_pOrig->CheckFeatureSupport(Feature, pFeatureSupportData, FeatureSupportDataSize);
    }
    virtual HRESULT __stdcall GetPrivateData(REFGUID guid, UINT* pDataSize, void* pData) override
    {
        return m_pOrig->GetPrivateData(guid, pDataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateData(REFGUID guid, UINT DataSize, const void* pData) override
    {
        return m_pOrig->SetPrivateData(guid, DataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateDataInterface(REFGUID guid, const IUnknown* pData) override
    {
        return m_pOrig->SetPrivateDataInterface(guid, pData);
    }
    virtual D3D_FEATURE_LEVEL __stdcall GetFeatureLevel(void) override
    {
        return m_pOrig->GetFeatureLevel();
    }
    virtual UINT __stdcall GetCreationFlags(void) override
    {
        return m_pOrig->GetCreationFlags();
    }
    virtual HRESULT __stdcall GetDeviceRemovedReason(void) override
    {
        return m_pOrig->GetDeviceRemovedReason();
    }
    virtual void __stdcall GetImmediateContext(ID3D11DeviceContext** ppImmediateContext) override
    {
        return m_pOrig->GetImmediateContext(ppImmediateContext);
    }
    virtual HRESULT __stdcall SetExceptionMode(UINT RaiseFlags) override
    {
        return m_pOrig->SetExceptionMode(RaiseFlags);
    }
    virtual UINT __stdcall GetExceptionMode(void) override
    {
        return m_pOrig->GetExceptionMode();
    }

private:
    ID3D11Device* m_pOrig;
};
