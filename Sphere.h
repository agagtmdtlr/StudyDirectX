#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{

public:
	Sphere();
	virtual ~Sphere();

	struct PrimitiveBuffer* GetBuffer();
	UINT GetIndexCount();
private:

	static std::unique_ptr<struct PrimitiveBuffer> buffer;
};

