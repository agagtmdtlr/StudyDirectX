#pragma once

#include "UI.h"
#include "SystemClass.h"

class UIManager
{
public:	
	UIManager(SystemClass* system);
	void Render();


	vector<unique_ptr<UI>> uis;
	SystemClass* system;
	
};

