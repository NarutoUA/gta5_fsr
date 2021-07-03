#pragma once

class ID3D11DeviceContextWrapper : public ID3D11DeviceContext
{
public:
    ID3D11DeviceContextWrapper(ID3D11DeviceContext* pOrigDeviceContext);

    ID3D11DeviceContextWrapper(const ID3D11DeviceContextWrapper&) = delete;
    ID3D11DeviceContextWrapper& operator=(const ID3D11DeviceContextWrapper) = delete;

    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObject) override
    {
        return m_pOrigDeviceContext->QueryInterface(riid, ppvObject);
    }
    virtual ULONG __stdcall AddRef(void) override
    {
        return m_pOrigDeviceContext->AddRef();
    }

    virtual ULONG __stdcall Release(void) override;

    virtual void __stdcall GetDevice(ID3D11Device** ppDevice) override
    {
        m_pOrigDeviceContext->GetDevice(ppDevice);
    }
    virtual HRESULT __stdcall GetPrivateData(REFGUID guid, UINT* pDataSize, void* pData) override
    {
        return m_pOrigDeviceContext->GetPrivateData(guid, pDataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateData(REFGUID guid, UINT DataSize, const void* pData) override
    {
        return m_pOrigDeviceContext->SetPrivateData(guid, DataSize, pData);
    }
    virtual HRESULT __stdcall SetPrivateDataInterface(REFGUID guid, const IUnknown* pData) override
    {
        return m_pOrigDeviceContext->SetPrivateDataInterface(guid, pData);
    }
    virtual void __stdcall VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }

    virtual void __stdcall PSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override;

    virtual void __stdcall PSSetShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall PSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall VSSetShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation) override
    {
        m_pOrigDeviceContext->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
    }

    virtual void __stdcall Draw(UINT VertexCount, UINT StartVertexLocation) override;

    virtual HRESULT __stdcall Map(ID3D11Resource* pResource, UINT Subresource, D3D11_MAP MapType, UINT MapFlags, D3D11_MAPPED_SUBRESOURCE* pMappedResource) override
    {
        return m_pOrigDeviceContext->Map(pResource, Subresource, MapType, MapFlags, pMappedResource);
    }
    virtual void __stdcall Unmap(ID3D11Resource* pResource, UINT Subresource) override
    {
        m_pOrigDeviceContext->Unmap(pResource, Subresource);
    }
    virtual void __stdcall PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall IASetInputLayout(ID3D11InputLayout* pInputLayout) override
    {
        m_pOrigDeviceContext->IASetInputLayout(pInputLayout);
    }
    virtual void __stdcall IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets) override
    {
        m_pOrigDeviceContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    }
    virtual void __stdcall IASetIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset) override
    {
        m_pOrigDeviceContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
    }
    virtual void __stdcall DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation) override
    {
        m_pOrigDeviceContext->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
    }
    virtual void __stdcall DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation) override
    {
        m_pOrigDeviceContext->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
    }
    virtual void __stdcall GSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall GSSetShader(ID3D11GeometryShader* pShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->GSSetShader(pShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology) override
    {
        m_pOrigDeviceContext->IASetPrimitiveTopology(Topology);
    }

    virtual void __stdcall VSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }

    virtual void __stdcall VSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall Begin(ID3D11Asynchronous* pAsync) override
    {
        m_pOrigDeviceContext->Begin(pAsync);
    }
    virtual void __stdcall End(ID3D11Asynchronous* pAsync) override
    {
        m_pOrigDeviceContext->End(pAsync);
    }
    virtual HRESULT __stdcall GetData(ID3D11Asynchronous* pAsync, void* pData, UINT DataSize, UINT GetDataFlags) override
    {
        return m_pOrigDeviceContext->GetData(pAsync, pData, DataSize, GetDataFlags);
    }
    virtual void __stdcall SetPredication(ID3D11Predicate* pPredicate, BOOL PredicateValue) override
    {
        m_pOrigDeviceContext->SetPredication(pPredicate, PredicateValue);
    }
    virtual void __stdcall GSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall GSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }

    virtual void __stdcall OMSetRenderTargets(UINT NumViews, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView) override;

    virtual void __stdcall OMSetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView* const* ppUnorderedAccessViews, const UINT* pUAVInitialCounts) override
    {
        m_pOrigDeviceContext->OMSetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, pDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    }
    virtual void __stdcall OMSetBlendState(ID3D11BlendState* pBlendState, const FLOAT BlendFactor[4], UINT SampleMask) override
    {
        m_pOrigDeviceContext->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
    }
    virtual void __stdcall OMSetDepthStencilState(ID3D11DepthStencilState* pDepthStencilState, UINT StencilRef) override
    {
        m_pOrigDeviceContext->OMSetDepthStencilState(pDepthStencilState, StencilRef);
    }
    virtual void __stdcall SOSetTargets(UINT NumBuffers, ID3D11Buffer* const* ppSOTargets, const UINT* pOffsets) override
    {
        m_pOrigDeviceContext->SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
    }
    virtual void __stdcall DrawAuto(void) override
    {
        m_pOrigDeviceContext->DrawAuto();
    }
    virtual void __stdcall DrawIndexedInstancedIndirect(ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) override
    {
        m_pOrigDeviceContext->DrawIndexedInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }
    virtual void __stdcall DrawInstancedIndirect(ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) override
    {
        m_pOrigDeviceContext->DrawInstancedIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }
    virtual void __stdcall Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ) override
    {
        m_pOrigDeviceContext->Dispatch(ThreadGroupCountX, ThreadGroupCountY, ThreadGroupCountZ);
    }
    virtual void __stdcall DispatchIndirect(ID3D11Buffer* pBufferForArgs, UINT AlignedByteOffsetForArgs) override
    {
        m_pOrigDeviceContext->DispatchIndirect(pBufferForArgs, AlignedByteOffsetForArgs);
    }
    virtual void __stdcall RSSetState(ID3D11RasterizerState* pRasterizerState) override
    {
        m_pOrigDeviceContext->RSSetState(pRasterizerState);
    }
    virtual void __stdcall RSSetViewports(UINT NumViewports, const D3D11_VIEWPORT* pViewports) override
    {
        m_pOrigDeviceContext->RSSetViewports(NumViewports, pViewports);
    }
    virtual void __stdcall RSSetScissorRects(UINT NumRects, const D3D11_RECT* pRects) override
    {
        m_pOrigDeviceContext->RSSetScissorRects(NumRects, pRects);
    }
    virtual void __stdcall CopySubresourceRegion(ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox) override
    {
        m_pOrigDeviceContext->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
    }
    virtual void __stdcall CopyResource(ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource) override
    {
        m_pOrigDeviceContext->CopyResource(pDstResource, pSrcResource);
    }
    virtual void __stdcall UpdateSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, const D3D11_BOX* pDstBox, const void* pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch) override
    {
        m_pOrigDeviceContext->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
    }
    virtual void __stdcall CopyStructureCount(ID3D11Buffer* pDstBuffer, UINT DstAlignedByteOffset, ID3D11UnorderedAccessView* pSrcView) override
    {
        m_pOrigDeviceContext->CopyStructureCount(pDstBuffer, DstAlignedByteOffset, pSrcView);
    }
    virtual void __stdcall ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView, const FLOAT ColorRGBA[4]) override
    {
        m_pOrigDeviceContext->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
    }
    virtual void __stdcall ClearUnorderedAccessViewUint(ID3D11UnorderedAccessView* pUnorderedAccessView, const UINT Values[4]) override
    {
        m_pOrigDeviceContext->ClearUnorderedAccessViewUint(pUnorderedAccessView, Values);
    }
    virtual void __stdcall ClearUnorderedAccessViewFloat(ID3D11UnorderedAccessView* pUnorderedAccessView, const FLOAT Values[4]) override
    {
        m_pOrigDeviceContext->ClearUnorderedAccessViewFloat(pUnorderedAccessView, Values);
    }
    virtual void __stdcall ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil) override
    {
        m_pOrigDeviceContext->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
    }
    virtual void __stdcall GenerateMips(ID3D11ShaderResourceView* pShaderResourceView) override
    {
        m_pOrigDeviceContext->GenerateMips(pShaderResourceView);
    }
    virtual void __stdcall SetResourceMinLOD(ID3D11Resource* pResource, FLOAT MinLOD) override
    {
        m_pOrigDeviceContext->SetResourceMinLOD(pResource, MinLOD);
    }
    virtual FLOAT __stdcall GetResourceMinLOD(ID3D11Resource* pResource) override
    {
        return m_pOrigDeviceContext->GetResourceMinLOD(pResource);
    }
    virtual void __stdcall ResolveSubresource(ID3D11Resource* pDstResource, UINT DstSubresource, ID3D11Resource* pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format) override
    {
        m_pOrigDeviceContext->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
    }
    virtual void __stdcall ExecuteCommandList(ID3D11CommandList* pCommandList, BOOL RestoreContextState) override
    {
        m_pOrigDeviceContext->ExecuteCommandList(pCommandList, RestoreContextState);
    }
    virtual void __stdcall HSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->HSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall HSSetShader(ID3D11HullShader* pHullShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->HSSetShader(pHullShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall HSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->HSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall HSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->HSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall DSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->DSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall DSSetShader(ID3D11DomainShader* pDomainShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->DSSetShader(pDomainShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall DSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->DSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall DSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->DSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall CSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->CSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall CSSetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView* const* ppUnorderedAccessViews, const UINT* pUAVInitialCounts) override
    {
        m_pOrigDeviceContext->CSSetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews, pUAVInitialCounts);
    }
    virtual void __stdcall CSSetShader(ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances) override
    {
        m_pOrigDeviceContext->CSSetShader(pComputeShader, ppClassInstances, NumClassInstances);
    }
    virtual void __stdcall CSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers) override
    {
        m_pOrigDeviceContext->CSSetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall CSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppConstantBuffers) override
    {
        m_pOrigDeviceContext->CSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall PSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall PSGetShader(ID3D11PixelShader** ppPixelShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->PSGetShader(ppPixelShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall PSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall VSGetShader(ID3D11VertexShader** ppVertexShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->VSGetShader(ppVertexShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall IAGetInputLayout(ID3D11InputLayout** ppInputLayout) override
    {
        m_pOrigDeviceContext->IAGetInputLayout(ppInputLayout);
    }
    virtual void __stdcall IAGetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppVertexBuffers, UINT* pStrides, UINT* pOffsets) override
    {
        m_pOrigDeviceContext->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
    }
    virtual void __stdcall IAGetIndexBuffer(ID3D11Buffer** pIndexBuffer, DXGI_FORMAT* Format, UINT* Offset) override
    {
        m_pOrigDeviceContext->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
    }
    virtual void __stdcall GSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall GSGetShader(ID3D11GeometryShader** ppGeometryShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->GSGetShader(ppGeometryShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall IAGetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY* pTopology) override
    {
        m_pOrigDeviceContext->IAGetPrimitiveTopology(pTopology);
    }
    virtual void __stdcall VSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall VSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall GetPredication(ID3D11Predicate** ppPredicate, BOOL* pPredicateValue) override
    {
        m_pOrigDeviceContext->GetPredication(ppPredicate, pPredicateValue);
    }
    virtual void __stdcall GSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall GSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall OMGetRenderTargets(UINT NumViews, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView) override
    {
        m_pOrigDeviceContext->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
    }
    virtual void __stdcall OMGetRenderTargetsAndUnorderedAccessViews(UINT NumRTVs, ID3D11RenderTargetView** ppRenderTargetViews, ID3D11DepthStencilView** ppDepthStencilView, UINT UAVStartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews) override
    {
        m_pOrigDeviceContext->OMGetRenderTargetsAndUnorderedAccessViews(NumRTVs, ppRenderTargetViews, ppDepthStencilView, UAVStartSlot, NumUAVs, ppUnorderedAccessViews);
    }
    virtual void __stdcall OMGetBlendState(ID3D11BlendState** ppBlendState, FLOAT BlendFactor[4], UINT* pSampleMask) override
    {
        m_pOrigDeviceContext->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
    }
    virtual void __stdcall OMGetDepthStencilState(ID3D11DepthStencilState** ppDepthStencilState, UINT* pStencilRef) override
    {
        m_pOrigDeviceContext->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
    }
    virtual void __stdcall SOGetTargets(UINT NumBuffers, ID3D11Buffer** ppSOTargets) override
    {
        m_pOrigDeviceContext->SOGetTargets(NumBuffers, ppSOTargets);
    }
    virtual void __stdcall RSGetState(ID3D11RasterizerState** ppRasterizerState) override
    {
        m_pOrigDeviceContext->RSGetState(ppRasterizerState);
    }
    virtual void __stdcall RSGetViewports(UINT* pNumViewports, D3D11_VIEWPORT* pViewports) override
    {
        m_pOrigDeviceContext->RSGetViewports(pNumViewports, pViewports);
    }
    virtual void __stdcall RSGetScissorRects(UINT* pNumRects, D3D11_RECT* pRects) override
    {
        m_pOrigDeviceContext->RSGetScissorRects(pNumRects, pRects);
    }
    virtual void __stdcall HSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->HSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall HSGetShader(ID3D11HullShader** ppHullShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->HSGetShader(ppHullShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall HSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->HSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall HSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->HSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall DSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->DSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall DSGetShader(ID3D11DomainShader** ppDomainShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->DSGetShader(ppDomainShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall DSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->DSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall DSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->DSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall CSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView** ppShaderResourceViews) override
    {
        m_pOrigDeviceContext->CSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
    }
    virtual void __stdcall CSGetUnorderedAccessViews(UINT StartSlot, UINT NumUAVs, ID3D11UnorderedAccessView** ppUnorderedAccessViews) override
    {
        m_pOrigDeviceContext->CSGetUnorderedAccessViews(StartSlot, NumUAVs, ppUnorderedAccessViews);
    }
    virtual void __stdcall CSGetShader(ID3D11ComputeShader** ppComputeShader, ID3D11ClassInstance** ppClassInstances, UINT* pNumClassInstances) override
    {
        m_pOrigDeviceContext->CSGetShader(ppComputeShader, ppClassInstances, pNumClassInstances);
    }
    virtual void __stdcall CSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D11SamplerState** ppSamplers) override
    {
        m_pOrigDeviceContext->CSGetSamplers(StartSlot, NumSamplers, ppSamplers);
    }
    virtual void __stdcall CSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer** ppConstantBuffers) override
    {
        m_pOrigDeviceContext->CSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
    }
    virtual void __stdcall ClearState(void) override
    {
        m_pOrigDeviceContext->ClearState();
    }
    virtual void __stdcall Flush(void) override
    {
        m_pOrigDeviceContext->Flush();
    }
    virtual D3D11_DEVICE_CONTEXT_TYPE __stdcall GetType(void) override
    {
        return m_pOrigDeviceContext->GetType();
    }
    virtual UINT __stdcall GetContextFlags(void) override
    {
        return m_pOrigDeviceContext->GetContextFlags();
    }
    virtual HRESULT __stdcall FinishCommandList(BOOL RestoreDeferredContextState, ID3D11CommandList** ppCommandList) override
    {
        return m_pOrigDeviceContext->FinishCommandList(RestoreDeferredContextState, ppCommandList);
    }

private:
    ID3D11DeviceContext* m_pOrigDeviceContext;

    UINT m_PSShaderResource_LastStartSlot;
    BOOL m_OMRenderTargets_Valid;
};
