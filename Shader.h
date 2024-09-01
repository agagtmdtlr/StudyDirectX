#pragma once
#include <d3d11.h>
#include <wrl/client.h>



class Shader
{
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
	// TODO:: Shader class ºÐ¸®
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;

	void InitShader();
};

