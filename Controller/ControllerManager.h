#pragma once

#include "Controller.h"
#include "Callback.h"
#include "ControllerFactory.h"

#include "Filebrowser.h"

struct ControllerLinkNode
{
	unique_ptr<Controller> ui;

	ControllerLinkNode* prev = nullptr;
	ControllerLinkNode* next = nullptr;

	void Insert(ControllerLinkNode* linkNode);
	ControllerLinkNode* Erase(ControllerLinkNode* linkNode);

	void Render();
};

class ControllerManager
{
public:
	static ControllerManager* g_uiManager;
	ControllerManager(class Renderer* renderer);

	void CreateUI(std::string type, std::string label);

	void Initialize();
	void Update();
	void Render();
	void CreateSphere();

	void SetCallbackToUI(std::string label, unique_ptr<Callback> callback);
	Controller* GetController(std::string label);

	vector<Controller*> uis;
	ControllerFactory factory;
	unordered_map<std::string, unique_ptr<Controller>> uimap;

	class Renderer* renderer = nullptr;

	FileBrowser fileBrowser;

};

