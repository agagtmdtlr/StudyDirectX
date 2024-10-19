#include "stdafx.h"
#include "ControllerManager.h"
#include "imgui.h"

#include "Level.h"
#include "Sphere.h"
#include "Button.h"
#include "Mesh.h"
#include "Renderer.h"

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
	CreateUI("gizmo", "gizmo");
	CreateUI("camera", "camera");
	ImGuiStyle& style = ImGui::GetStyle();

	{
		
	}
	ImGui::StyleColorsDark(&style);
}

void ControllerManager::UpdateControllerManager()
{
	ImGui::DockSpaceOverViewport();

	ImGui::Begin("Option");

	ImVec2 pos = ImGui::GetMousePos();
	ImGui::InputFloat2("Mouse Pos", &pos.x);

	Ray ray = renderer->camera.ScreenPointToRay(Vector2(pos.x, pos.y));
	ImGui::InputFloat3("ray pos", &ray.position.x);
	ImGui::InputFloat3("ray dir", &ray.direction.x);


	if ( ImGui::IsMouseClicked(ImGuiMouseButton_Left) == true )
	{
		Mesh* mesh = renderer->GetSphere();
		bool intersect = mesh->RayCast(ray);
		if (intersect == true)
		{
			GetController("gizmo")->model = mesh ;
		}
	}
	
	if (ImGui::IsKeyPressed(ImGuiKey_Escape) == true)
	{
		GetController("gizmo")->model.reset();
	}

	for (auto c : uis)
	{
		c->Render();
	}
	ImGui::End();


	fileBrowser.Render();



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

