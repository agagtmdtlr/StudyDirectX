#pragma once
#include "Sphere.h"

class Mesh;
class ControllerManager;
class Application
{
public:
	Application(ControllerManager* uiManager);
	void Initialize();
	Mesh* GetMesh();
	Sphere sphere;

	ControllerManager* uiManager;
};

