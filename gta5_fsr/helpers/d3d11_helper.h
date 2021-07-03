#pragma once

template<unsigned int N>
HRESULT CreateComputeShader(ID3D11Device* pDevice, const unsigned char(&pByteCode)[N], ID3D11ComputeShader** ppShaderOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    return pDevice->CreateComputeShader(pByteCode, N, nullptr, ppShaderOut);
}

template <class T>
HRESULT CreateConstantBuffer(ID3D11Device* pDevice, const T* pInitData, ID3D11Buffer** ppBufOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    *ppBufOut = nullptr;

    D3D11_BUFFER_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ByteWidth = sizeof(T);
    desc.Usage = D3D11_USAGE_DYNAMIC;
    desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    desc.MiscFlags = 0;
    desc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = pInitData;
    InitData.SysMemPitch = 0;
    InitData.SysMemSlicePitch = 0;

    return pDevice->CreateBuffer(&desc, &InitData, ppBufOut);
}

HRESULT CreateTextureSRV(ID3D11Device* pDevice, ID3D11Texture2D* pTexture, ID3D11ShaderResourceView** ppSRVOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    D3D11_TEXTURE2D_DESC descTex;
    ZeroMemory(&descTex, sizeof(descTex));
    pTexture->GetDesc(&descTex);

    D3D11_SHADER_RESOURCE_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    desc.Format = descTex.Format;
    desc.Texture2D.MipLevels = 1;
    desc.Texture2D.MostDetailedMip = 0;

    return pDevice->CreateShaderResourceView(pTexture, &desc, ppSRVOut);
}

HRESULT CreateTextureUAV(ID3D11Device* pDevice, ID3D11Texture2D* pTexture, ID3D11UnorderedAccessView** ppUAVOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    D3D11_TEXTURE2D_DESC descTex;
    ZeroMemory(&descTex, sizeof(descTex));
    pTexture->GetDesc(&descTex);

    D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
    desc.Format = descTex.Format;
    desc.Texture2D.MipSlice = 0;

    return pDevice->CreateUnorderedAccessView(pTexture, &desc, ppUAVOut);
}

HRESULT CreateTextureRTV(ID3D11Device* pDevice, ID3D11Texture2D* pTexture, ID3D11RenderTargetView** ppRTVOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    D3D11_TEXTURE2D_DESC descTex;
    ZeroMemory(&descTex, sizeof(descTex));
    pTexture->GetDesc(&descTex);

    D3D11_RENDER_TARGET_VIEW_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    desc.Format = descTex.Format;
    desc.Texture2D.MipSlice = 0;

    return pDevice->CreateRenderTargetView(pTexture, &desc, ppRTVOut);
}

HRESULT CreateSampler(ID3D11Device* pDevice, ID3D11SamplerState** ppSamplerOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    D3D11_SAMPLER_DESC SamplerDesc;
    ZeroMemory(&SamplerDesc, sizeof(SamplerDesc));
    SamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    SamplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
    SamplerDesc.MipLODBias = 0;
    SamplerDesc.MinLOD = 0.f;
    SamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    return pDevice->CreateSamplerState(&SamplerDesc, ppSamplerOut);
}

HRESULT CreateDestinationTexture(ID3D11Device* pDevice, const D3D11_TEXTURE2D_DESC& desc, ID3D11Texture2D** ppTextureOut)
{
    if (pDevice == nullptr)
        return E_FAIL;

    D3D11_TEXTURE2D_DESC TextureDesc;
    ZeroMemory(&TextureDesc, sizeof(TextureDesc));
    TextureDesc.Format = desc.Format;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.CPUAccessFlags = 0;
    TextureDesc.MiscFlags = 0;
    TextureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET | D3D11_BIND_UNORDERED_ACCESS;
    TextureDesc.Width = desc.Width;
    TextureDesc.Height = desc.Height;
    TextureDesc.Usage = D3D11_USAGE_DEFAULT;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.SampleDesc.Quality = 0;

    return pDevice->CreateTexture2D(&TextureDesc, nullptr, ppTextureOut);
}

HRESULT RunComputeShader(ID3D11DeviceContext* pDeviceContext, ID3D11ComputeShader* pCS, ID3D11ShaderResourceView* pSRV, ID3D11Buffer* pCBCS, ID3D11UnorderedAccessView* pUAV, ID3D11SamplerState* pSampler, UINT X, UINT Y, UINT Z)
{
    if (pDeviceContext == nullptr)
        return E_FAIL;

    pDeviceContext->CSSetShader(pCS, NULL, 0);
    pDeviceContext->CSSetShaderResources(0, 1, &pSRV);
    pDeviceContext->CSSetUnorderedAccessViews(0, 1, &pUAV, NULL);
    pDeviceContext->CSSetConstantBuffers(0, 1, &pCBCS);

    if (pSampler)
        pDeviceContext->CSSetSamplers(0, 1, &pSampler);

    pDeviceContext->Dispatch(X, Y, Z);

    pDeviceContext->CSSetShader(NULL, NULL, 0);

    ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
    pDeviceContext->CSSetUnorderedAccessViews(0, 1, ppUAViewNULL, NULL);

    ID3D11ShaderResourceView* ppSRVNULL[1] = { NULL };
    pDeviceContext->CSSetShaderResources(0, 1, ppSRVNULL);

    ID3D11Buffer* ppCB[1] = { pCBCS };
    pDeviceContext->CSSetConstantBuffers(0, 1, ppCB);

    ID3D11Buffer* ppCBNULL[1] = { NULL };
    pDeviceContext->CSSetConstantBuffers(0, 1, ppCBNULL);

    ID3D11SamplerState* ppSSNULL[1] = { NULL };
    pDeviceContext->CSSetSamplers(0, 1, ppSSNULL);

    return S_OK;
}

template <class T>
std::unique_ptr<T> ReadConstantBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext, ID3D11Buffer* pBuffer)
{
    if (pDevice == nullptr || pDeviceContext == nullptr || pBuffer == nullptr)
        return nullptr;

    D3D11_BUFFER_DESC desc;
    pBuffer->GetDesc(&desc);

    wil::com_ptr_t<ID3D11Buffer> pReadBuffer;
    pReadBuffer.attach(pBuffer);
    pReadBuffer->AddRef();

    if ((desc.CPUAccessFlags & D3D11_CPU_ACCESS_READ) == 0)
    {
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        desc.Usage = D3D11_USAGE_STAGING;
        desc.BindFlags = 0;
        desc.MiscFlags = 0;

        ID3D11Buffer* pTmpBuffer = nullptr;
        if (FAILED(pDevice->CreateBuffer(&desc, nullptr, &pTmpBuffer)))
            return nullptr;

        pReadBuffer.attach(pTmpBuffer);
        pDeviceContext->CopyResource(pReadBuffer.get(), pBuffer);
    }

    D3D11_MAPPED_SUBRESOURCE resource;
    if (SUCCEEDED(pDeviceContext->Map(pReadBuffer.get(), 0, D3D11_MAP_READ, 0, &resource)))
    {
        if (resource.RowPitch < sizeof(T))
            return nullptr;

        T* pData = new T();
        memcpy(pData, resource.pData, sizeof(T));

        std::unique_ptr<T> result;
        result.reset(pData);

        pDeviceContext->Unmap(pReadBuffer.get(), 0);

        return result;
    }

    return nullptr;
}
