#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{

public:
	Sphere();
	virtual ~Sphere();

	class PrimitiveBuffer* GetBuffer();
	UINT GetIndexCount();
private:

	static std::unique_ptr<class PrimitiveBuffer> buffer;
};



