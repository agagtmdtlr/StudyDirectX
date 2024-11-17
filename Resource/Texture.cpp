#include "stdafx.h"
#include "Texture.h"
#include "D3D.h"

Texture::Texture(TexMetadata metaData, ID3D11ShaderResourceView* srv)
	:metaData(metaData), srv(srv)
{
}

Texture::Texture(TexMetadata& metaData, DirectX::ScratchImage& simg)
    :metaData(metaData)
{
    auto device=  D3D::GetDevice();

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = metaData.width;
    desc.Height = metaData.height;
    desc.MipLevels = metaData.mipLevels;
    desc.ArraySize = metaData.arraySize;
    desc.Format = metaData.format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 1;

    DirectX::CreateShaderResourceViewEx(
        device,
        simg.GetImages(),
        simg.GetImageCount(),
        metaData,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE,
        0, 0, CREATETEX_DEFAULT, srv.GetAddressOf());
}

Texture::Texture(TexMetadata& metaData)
    :metaData(metaData)
{
    auto device = D3D::GetDevice();

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = metaData.width;
    desc.Height = metaData.height;
    desc.MipLevels = metaData.mipLevels;
    desc.ArraySize = metaData.arraySize;
    desc.Format = metaData.format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 1;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE,
    desc.Usage = D3D11_USAGE_DEFAULT;

    ComPtr<ID3D11Texture2D> tex;
    device->CreateTexture2D(&desc,nullptr,tex.GetAddressOf());

    D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
    ZeroMemory(&srvdesc, sizeof(srvdesc));
    srvdesc.Format = metaData.format;
    srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvdesc.Texture2D.MipLevels = metaData.mipLevels;
    device->CreateShaderResourceView(tex.Get(), & srvdesc, srv.GetAddressOf() );

}
