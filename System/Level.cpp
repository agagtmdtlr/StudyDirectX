#include "stdafx.h"
#include "Level.h"
#include "Mesh.h"
#include "Sphere.h"

Level::Level(string name)
{
}

Mesh* Level::CreateSphere()
{
	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	meshes.push_back(sphere);
	return sphere.get();
}



