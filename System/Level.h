#pragma once
#include "Callback.h"

class Mesh;
class Level
{
public:
	Level(string name);
	Mesh* CreateSphere();
private:
	string name;

	vector<shared_ptr<Mesh>> meshes;
};

