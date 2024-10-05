#pragma once

#include "UI.h"
#include "Callback.h"
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
	UIManager(class Renderer* renderer);

	void Initialize();
	void Update();
	void Render();
	void CreateSphere();

	bool IsSelected();

	void SetCallbackToUI(std::string label, unique_ptr<Callback> callback);

	UILinkNode* header;
	vector<unique_ptr<UI>> uis;

	unordered_map<std::string, UI*> uimap;
	class Renderer* renderer;
	class Application* application;
	
	class Mesh* selectedObject;
	
};

