#include "stdafx.h"

#include "ID3D11DeviceWrapper.h"

ID3D11DeviceWrapper::ID3D11DeviceWrapper(ID3D11Device* pOrigDevice)
    : m_pOrig(pOrigDevice)
{
}

ULONG ID3D11DeviceWrapper::Release()
{
    auto count = m_pOrig->Release();

    if (count == 0)
        delete this;

    return count;
}

HRESULT __stdcall ID3D11DeviceWrapper::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT* pNumQualityLevels)
{
    // weird way to check if menu was opened
    CSuperResolutionMgr::UpdateMSAA();
    return m_pOrig->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}
