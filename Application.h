#pragma once
#include "Sphere.h"

class Application
{
public:
	Application(UIManager* uiManager);
	void Initialize();
	class Mesh* GetMesh();
	Sphere sphere;

	class UIManager* uiManager;
};

