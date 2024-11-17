#include "stdafx.h"
#include "Level.h"
#include "Mesh.h"
#include "Sphere.h"
#include "ServiceLoactor.h"
#include "Renderer.h"

Level::Level()
{

}

void Level::LoadLevel(string path)
{
	ServiceLoactor::RegisterService<Level>(this);

	string fullpath = "../Resources/level/";
}

void Level::AddMesh(shared_ptr<Mesh> mesh)
{
	meshes.push_back(mesh);
}

void Level::UpdateScene()
{
	Renderer* renderer = ServiceLoactor::GetService<Renderer>();
	for (auto m : meshes)
	{
		renderer->reserveDrawMesh(m);
	}
}

Mesh* Level::RayCast(Ray ray)
{
	Mesh* mesh = nullptr;
	for (auto m : meshes)
	{
		if (m->RayCast(ray))
		{
			mesh = m.get();
		}
		//mesh = m.get();

	}
	return mesh;
}

Mesh* Level::CreateSphere()
{
	shared_ptr<Sphere> sphere = make_shared<Sphere>();
	sphere->LoadMesh("");
	sphere->Initialize(Transform());
	meshes.push_back(sphere);
	return sphere.get();
}



