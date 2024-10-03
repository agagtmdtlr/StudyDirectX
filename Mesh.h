#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <set>

#include "Material.h"

class Mesh
{
public:
	Mesh();
	Mesh(string name, Transform transform);
	virtual ~Mesh();
	virtual void Update();

	Transform* GetTransformRef() { return &transform; }
	Material::Description GetMaterialDesctription() { return material.desc; }
private:
	void Initialize(std::string name, Transform transform);
	void Create();
	UINT id;
	Transform transform;
	string name;
	Material material;

	static UINT idGenerator;
	static set<UINT> instancesUID;
};

