#pragma once
#include <d3d11.h>
#include <wrl/client.h>

#include "Transform.h"
#include <set>

#include "Material.h"

class Mesh
{
public:
	Mesh();
	virtual ~Mesh();
	virtual void Update();
	virtual void initialize(string name, Transform transform);
	
	Transform* GetTransformRef() { return &transform;}
	Material::Description GetMaterialDesctription() {return material.desc; }
private:
	UINT id;
	Transform transform;
	string name;
	Material material;

	static UINT idGenerator;
	static set<UINT> instancesUID;
};

