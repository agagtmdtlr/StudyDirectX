#pragma once

#include <windows.h>

#include "RenderPass.h"
#include "Mesh.h"
#include <memory>

#include "Camera.h"
#include "ConstantBuffer.h"
#include "Sphere.h"
#include "Material.h"

class Renderer
{
public:

	Renderer();
	void Initialize(int width, int height);
	void Update();
	
	void Render();

	std::unique_ptr<RenderPass> renderPass;
	std::unique_ptr<Sphere> sphere;

	std::array<Material,1024> materials;
	
	shared_ptr<PrimitiveBuffer> primitive; // for test
	shared_ptr<class Texture> texture;
	ComPtr<ID3D11SamplerState> colorSampler;

	Camera camera;
};