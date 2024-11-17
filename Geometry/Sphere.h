#pragma once
#include "Mesh.h"

class Sphere : public Mesh
{

public:
	Sphere();
	virtual ~Sphere();
	virtual void LoadMesh(string path);

	virtual bool RayCast(const DirectX::SimpleMath::Ray& ray);
};

class Cube : public Mesh
{
public:
	Cube();
	virtual ~Cube() {}

	virtual void LoadMesh(string path);
};



