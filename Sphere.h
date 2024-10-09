#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{

public:
	Sphere();
	virtual ~Sphere();

	virtual bool RayCast(const Ray& ray);

};



