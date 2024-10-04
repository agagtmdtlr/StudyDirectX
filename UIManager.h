#pragma once

#include "UI.h"
#include "SystemClass.h"

struct UILinkNode
{
	unique_ptr<UI> ui;

	UILinkNode* prev = nullptr;
	UILinkNode* next = nullptr;

	void Insert(UILinkNode* linkNode);
	UILinkNode* Erase(UILinkNode* linkNode);

	void Render();
};

class UIManager
{
public:
	static UIManager* g_uiManager;
	UIManager(SystemClass* system, Renderer* renderer);
	void Initialize();
	void Render();
	void CreateSphere();

	bool IsSelected();

	UILinkNode* header;
	vector<unique_ptr<UI>> uis;
	SystemClass* system;
	Renderer* renderer;

	class Mesh* selectedObject;
	
};

