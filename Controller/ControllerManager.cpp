#include "stdafx.h"
#include "ControllerManager.h"
#include "imgui.h"

#include "Level.h"
#include "Sphere.h"
#include "Button.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Level.h"

ControllerManager* ControllerManager::g_uiManager = nullptr;

ControllerManager::ControllerManager(Renderer* renderer)
	: renderer(renderer)
{
}

void ControllerManager::CreateUI(std::string type, std::string label)
{
	uimap[label] = std::move(factory.CreateUI(type));
	auto& ui = uimap[label];
	ui->label = label;
	uis.push_back(ui.get());
}




void ControllerManager::Initialize()
{
	g_uiManager = this;

	// TODO:: UI 레이아웃을 css 구조를 통해 사전에 정의하다록 변경, 여기에 있는 하드코딩 제거
	CreateUI("material", "material");
	//CreateUI("gizmo", "gizmo");
	CreateUI("camera", "camera");
	ImGuiStyle& style = ImGui::GetStyle();

	{
		
	}
	ImGui::StyleColorsDark(&style);
}

void ControllerManager::UpdateControllerManager()
{

	ImGui::Begin("Game Option");

	ImVec2 pos = ImGui::GetMousePos();
	if(ImGui::IsMousePosValid())
		ImGui::InputFloat2("Mouse Pos", &pos.x);

	Ray ray = renderer->camera.ScreenPointToRay(Vector2(pos.x, pos.y));
	ImGui::InputFloat3("ray pos", &ray.position.x);
	ImGui::InputFloat3("ray dir", &ray.direction.x);


	for (auto c : uis)
	{
		c->Render();
	}
	ImGui::End();


	fileBrowser.Render();
	sceneDisplayer.Render();


	ImGui::Begin("Debug");
	{
		ImGui::Text("Size %d %d", D3D::GetWidth(), D3D::GetHeight());
		ImGui::Text("Display Size %d %d", D3D::GetDisplayWidth(), D3D::GetDisplayHeight());
		ImGui::Text("Aspect %f", D3D::GetDisplayAspectRatio());
		//ImGui::Begin("viewport", 0, wflag);		
	}
	ImGui::End();


	//ImGui::ShowDemoWindow();
}

void ControllerManager::Render()
{
	
}

void ControllerManager::CreateSphere()
{
}


void ControllerManager::SetCallbackToUI(std::string label, unique_ptr<Callback> callback)
{
	if (uimap.find(label) != uimap.end())
	{
		uimap[label]->callback = std::move(callback);
	}
}

Controller* ControllerManager::GetController(std::string label)
{
	return uimap[label].get();
}

void ControllerLinkNode::Insert(ControllerLinkNode* linkNode)
{
	if (next != nullptr)
	{
		next->prev = linkNode;
		linkNode->next = next;
		linkNode->prev = this;
	}

	this->next = linkNode;
}

ControllerLinkNode* ControllerLinkNode::Erase(ControllerLinkNode* linkNode)
{
	if (linkNode != this)
	{
		linkNode->next->Erase(linkNode);
	}
	else
	{
		if (prev != nullptr)
		{
			prev->next = next;
		}

		if (next != nullptr)
		{
			next->prev = prev;
		}
	}

	return this;
}

void ControllerLinkNode::Render()
{

	if (ui != nullptr)
	{
		ui->Render();
	}

	if (next != nullptr)
	{
		next->Render();
	}
}

