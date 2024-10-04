#pragma once

#include "UI.h"


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
	UIManager(ApplicationClass* app, Renderer* renderer);

	void Initialize();
	void Update();
	void Render();
	void CreateSphere();

	bool IsSelected();


	UILinkNode* header;
	vector<unique_ptr<UI>> uis;
	Renderer* renderer;
	ApplicationClass* application;

	class Mesh* selectedObject;
	
};

