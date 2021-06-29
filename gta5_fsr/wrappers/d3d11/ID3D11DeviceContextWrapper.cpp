#include "stdafx.h"

#include "ID3D11DeviceContextWrapper.h"

constexpr UINT INVALID_START_SLOT = 0xDEADC0DE;

ID3D11DeviceContextWrapper::ID3D11DeviceContextWrapper(ID3D11DeviceContext* pOrigDeviceContext)
    : m_pOrigDeviceContext(pOrigDeviceContext)
    , m_PSShaderResource_LastStartSlot(INVALID_START_SLOT)
    , m_OMRenderTargets_Valid(FALSE)
    , m_VSShaderResource_Valid(FALSE)
{

}

ULONG __stdcall ID3D11DeviceContextWrapper::Release(void)
{
    auto count = m_pOrigDeviceContext->Release();

    if (count == 0)
        delete this;

    return count;
}

void __stdcall ID3D11DeviceContextWrapper::PSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
    if (NumViews == 1 && ppShaderResourceViews != nullptr)
    {
        m_PSShaderResource_LastStartSlot = StartSlot;
    }
    else
    {
        m_PSShaderResource_LastStartSlot = INVALID_START_SLOT;
    }
    m_pOrigDeviceContext->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void __stdcall ID3D11DeviceContextWrapper::Draw(UINT VertexCount, UINT StartVertexLocation)
{
    auto skip_orig_call = false;

    if (VertexCount == 3 && StartVertexLocation == 3
        && m_PSShaderResource_LastStartSlot != INVALID_START_SLOT
        && m_OMRenderTargets_Valid && m_VSShaderResource_Valid)
    {
        wil::com_ptr_t<ID3D11ShaderResourceView> src;
        m_pOrigDeviceContext->PSGetShaderResources(m_PSShaderResource_LastStartSlot, 1, src.addressof());

        wil::com_ptr_t<ID3D11RenderTargetView> dst;
        m_pOrigDeviceContext->OMGetRenderTargets(1, dst.addressof(), nullptr);

        if (CSuperResolutionMgr::RunSuperResolutionPass(src, dst))
            skip_orig_call = true;
    }

    m_PSShaderResource_LastStartSlot = INVALID_START_SLOT;
    m_OMRenderTargets_Valid = FALSE;
    m_VSShaderResource_Valid = FALSE;

    if (skip_orig_call == false)
        m_pOrigDeviceContext->Draw(VertexCount, StartVertexLocation);
}

void __stdcall ID3D11DeviceContextWrapper::VSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D11ShaderResourceView* const* ppShaderResourceViews)
{
    m_VSShaderResource_Valid = NumViews == 1 && ppShaderResourceViews != nullptr;
    m_pOrigDeviceContext->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

void __stdcall ID3D11DeviceContextWrapper::OMSetRenderTargets(UINT NumViews, ID3D11RenderTargetView* const* ppRenderTargetViews, ID3D11DepthStencilView* pDepthStencilView)
{
    m_OMRenderTargets_Valid = NumViews == 1 && ppRenderTargetViews != nullptr && pDepthStencilView == nullptr;
    m_pOrigDeviceContext->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}
