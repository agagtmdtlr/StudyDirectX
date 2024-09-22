#pragma once

#include <windows.h>

#include "RenderPass.h"
#include "Mesh.h"
#include <memory>

class RenderDemo
{
public:

	RenderDemo(HWND window, int width , int height);
	void Initialize(HWND window, int width, int height);
	void Update();
	void Render();

	std::unique_ptr<RenderPass> renderPass;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	ComPtr<ID3D11Texture2D> canvasTexture;

	ComPtr<ID3D11ShaderResourceView> canvasTextureView;
	ComPtr<ID3D11RenderTargetView> canvasRenderTargetView;
	ComPtr<ID3D11SamplerState> colorSampler;

	std::vector<Mesh> meshes;

	UINT indexCount;
};