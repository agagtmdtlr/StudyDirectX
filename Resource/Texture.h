#pragma once
#include <DirectXTex.h>

class Texture
{
public:
	Texture(TexMetadata metaData, ID3D11ShaderResourceView* srv);
	Texture(TexMetadata& metaData, DirectX::ScratchImage& simg);
	Texture(TexMetadata& metaData);


	ID3D11ShaderResourceView* GetSRV() {return srv.Get(); }
	ID3D11RenderTargetView* GetRTV() { return rtv.Get(); }
	ID3D11DepthStencilView* GetDSV() { return dsv.Get(); }

private:

	DirectX::TexMetadata metaData;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsv;
};


