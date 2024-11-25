#pragma once
#include "Material.h"

class Mesh
{
public:

	Mesh();
	virtual ~Mesh();
	virtual void LoadMesh(string path) = 0;
	virtual void Update();
	void RenderMesh();
	
	virtual void Initialize(Transform transform);
	virtual bool RayCast(const Ray& ray) {return false;}
	virtual std::string GetBufferName() { return std::string(typeid(*this).name()); }


	void SetWorldMatrix(Matrix& matrix);
	Transform* GetTransformRef() { return &transform; }

public:
	class PrimitiveBuffer* GetBuffer(UINT index);
	UINT GetBufferCount();

protected:
	shared_ptr<class PrimitiveBufferContainer> buffer;
	vector<Material> materials;
	Transform transform;
	BoundingBox aabb;
	string name;

private:
	UINT id;	


	static UINT idGenerator;
	static set<UINT> instancesUID;
	
};

