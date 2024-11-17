#pragma once

class Mesh;
class ControllerManager;
class Application
{
public:
	Application(ControllerManager* uiManager);
	virtual void InitializeApplication();
	virtual void UpdateApplication();

	ControllerManager* uiManager;
};

