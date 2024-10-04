#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <set>

#include "Material.h"

class Mesh
{
public:

	Mesh();
	virtual ~Mesh();
	virtual void Update();
	virtual void Render();
	virtual void Initialize(std::string name, Transform transform);

	virtual std::string GetBufferName() { return std::string(typeid(*this).name()); }


	Transform* GetTransformRef() { return &transform; }
	Material::Description GetMaterialDesctription() { return material.desc; }

public:
	class PrimitiveBuffer* GetBuffer();
	UINT GetIndexCount();

protected:
	shared_ptr<PrimitiveBuffer> buffer;
private:
	UINT id;
	Transform transform;
	string name;
	Material material;


	static UINT idGenerator;
	static set<UINT> instancesUID;
	
};

