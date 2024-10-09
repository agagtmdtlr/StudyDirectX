#pragma once

#include <windows.h>

#include "Shader.h"
#include "Mesh.h"
#include <memory>

#include "Camera.h"
#include "ConstantBuffer.h"
#include "Sphere.h"
#include "Material.h"

class Renderer
{
public:
	static Renderer* g_renderer;
	Renderer();
	void Initialize(int width, int height);
	void Update();
	void UpdateCamera(const Camera& camera);
	
	void Render();

	Mesh* GetSphere() { return sphere.get(); }

	std::unique_ptr<Shader> renderPass;

	std::array<Material,1024> materials;
	
	shared_ptr<class Texture> texture;
	ComPtr<ID3D11SamplerState> colorSampler;

	shared_ptr<PrimitiveBuffer> primitive;
	std::unique_ptr<Sphere> sphere;

	Camera camera;
};