#pragma once

#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include "RenderCommon.h"
#include <wrl/client.h>
#include "Mesh.h"

using namespace Microsoft::WRL;

class RenderContext
{	
public:
	static RenderContext* GetRenderContext();

	void Initialize(HWND window, int width, int height);
	void InitShaders();

	void Render();

	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	D3D11_VIEWPORT viewport;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	
	// TODO:: Shader class �и�
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> layout;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Texture2D> canvasTexture;

	ComPtr<ID3D11ShaderResourceView> canvasTextureView;
	ComPtr<ID3D11RenderTargetView> canvasRenderTargetView;
	ComPtr<ID3D11SamplerState> colorSampler;


	std::vector<Mesh> meshes;
	

	int width, height;

};
