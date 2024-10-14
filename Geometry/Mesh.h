#pragma once
#include "Material.h"

class Mesh
{
public:

	Mesh();
	virtual ~Mesh();
	virtual void Update();
	virtual void Render();
	virtual void Initialize(std::string name, Transform transform);

	virtual bool RayCast(const Ray& ray) {return false;}

	virtual std::string GetBufferName() { return std::string(typeid(*this).name()); }


	void SetWorldMatrix(Matrix& matrix);
	Transform* GetTransformRef() { return &transform; }
	Material::Description GetMaterialDesctription() { return material.desc; }

public:
	class PrimitiveBuffer* GetBuffer();
	UINT GetIndexCount();

protected:
	shared_ptr<PrimitiveBuffer> buffer;
	Transform transform;
private:
	UINT id;
	string name;
	Material material;


	static UINT idGenerator;
	static set<UINT> instancesUID;
	
};

