#include "stdafx.h"
#include "ControllerManager.h"
#include "imgui.h"
#include "Level.h"
#include "Sphere.h"
#include "ButtonController.h"
#include "Mesh.h"
#include "Renderer.h"

ControllerManager* ControllerManager::g_uiManager = nullptr;

ControllerManager::ControllerManager(Renderer* renderer)
	: renderer(renderer), selectedObject(nullptr)
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
	// TODO:: UI ���̾ƿ��� css ������ ���� ������ �����ϴٷ� ����, ���⿡ �ִ� �ϵ��ڵ� ����

	{
		CreateUI("button","Create Mesh");
	}
	{
		CreateUI("button", "Delete Mesh");
	}
	{
		CreateUI("material", "Material");
	}

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);

}

void ControllerManager::Update()
{
	if (ImGui::IsKeyPressed(ImGuiKey_1) == true)
	{
	}
}

void ControllerManager::Render()
{

}

void ControllerManager::CreateSphere()
{
}

bool ControllerManager::IsSelected()
{
	return selectedObject != nullptr;
}

void ControllerManager::SetCallbackToUI(std::string label, unique_ptr<Callback> callback)
{
	if (uimap.find(label) != uimap.end())
	{
		uimap[label]->callback = std::move(callback);
	}
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
