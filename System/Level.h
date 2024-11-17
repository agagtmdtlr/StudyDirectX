#pragma once
#include "Callback.h"

#include "Camera.h"
class Mesh;
class Level
{
public:

	Level();
	void LoadLevel(string path);
	void AddMesh(shared_ptr<Mesh> mesh);
	void UpdateScene();
	Mesh* RayCast(Ray ray);
	Mesh* CreateSphere();
private:
	string name;

	vector<shared_ptr<Mesh>> meshes;

	Camera camera;
};

