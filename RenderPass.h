#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <string>


struct RenderPass
{
	D3D11_VIEWPORT viewport;

	
	ComPtr<ID3D11RenderTargetView> rtv;

	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;


	RenderPass(std::wstring shaderName);

	void Initialize(std::wstring shaderName);

};

