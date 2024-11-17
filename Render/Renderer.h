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
	Renderer();

	void Initialize(int width, int height);
	void Update();
	void UpdateCamera(const Camera& camera);
	
	void reserveDrawMesh(shared_ptr<Mesh> mesh);
	void Render();
	void BeginUI();

	void Resize(int width, int height);


	std::unique_ptr<Shader> renderPass;

	std::array<Material,1024> materials;
	
	shared_ptr<class Texture> texture;
	ComPtr<ID3D11SamplerState> colorSampler;

	shared_ptr<PrimitiveBuffer> primitive;

	std::vector<shared_ptr<Mesh>> meshesForDraw;

	Camera camera;
};