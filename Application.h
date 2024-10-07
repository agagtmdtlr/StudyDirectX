#pragma once
#include "Sphere.h"
#include "Camera.h"

class Mesh;
class ControllerManager;
class Application
{
public:
	Application(ControllerManager* uiManager);
	void Initialize();
	Mesh* GetMesh();
	Sphere sphere;

	Camera camera;
	ControllerManager* uiManager;
};

