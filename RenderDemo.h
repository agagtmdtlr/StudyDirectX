#pragma once

#include <windows.h>

#include "RenderPass.h"
#include "Mesh.h"
#include <memory>


#include "Camera.h"
#include "ConstantBuffer.h"
#include "Sphere.h"
class RenderDemo
{
public:

	RenderDemo();
	void Initialize(int width, int height);
	void Update();
	
	void Render();

	std::unique_ptr<RenderPass> renderPass;
	Sphere sphere;
	
	shared_ptr<PrimitiveBuffer> primitive;
	shared_ptr<class Texture> texture;
	ComPtr<ID3D11SamplerState> colorSampler;

	Camera camera;
};