#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{

public:
	Sphere();
	Sphere(std::string name, Transform transform);
	virtual ~Sphere();

	struct PrimitiveBuffer* GetBuffer();
	void Create();
	UINT GetIndexCount();
private:

	static std::unique_ptr<struct PrimitiveBuffer> buffer;
};

