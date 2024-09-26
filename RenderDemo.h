#pragma once

#include <windows.h>

#include "RenderPass.h"
#include "Mesh.h"
#include <memory>


#include "Camera.h"
#include "ConstantBuffer.h"
class RenderDemo
{
public:

	RenderDemo();
	void Initialize(int width, int height);
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

	Camera camera;

	UINT indexCount;
};