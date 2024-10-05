#include "stdafx.h"
#include "UIManager.h"
#include "imgui.h"
#include "Level.h"
#include "Sphere.h"
#include "UIButton.h"
#include "Mesh.h"
#include "Renderer.h"

UIManager* UIManager::g_uiManager = nullptr;

UIManager::UIManager(Renderer* renderer)
	: renderer(renderer), header(nullptr), selectedObject(nullptr)
{
}

void UIManager::Initialize()
{
	g_uiManager = this;

	header = new UILinkNode();
	{
		UILinkNode* add = new UILinkNode();
		add->ui = make_unique<UIButton>();
		UIButton* btn = dynamic_cast<UIButton*>(add->ui.get());
		btn->label = "Create Mesh";
		header->Insert(add);
	}
	{
		UILinkNode* del = new UILinkNode();
		del->ui = make_unique<UIButton>();
		UIButton* btn = dynamic_cast<UIButton*>(del->ui.get());
		btn->label = "Delete Mesh";
		header->Insert(del);
	}

	ImGuiStyle& style = ImGui::GetStyle();
	ImGui::StyleColorsDark(&style);

}

void UIManager::Update()
{
	if (ImGui::IsKeyPressed(ImGuiKey_1) == true)
	{
	}
}

void UIManager::Render()
{

	header->Render();

}

void UIManager::CreateSphere()
{
}

bool UIManager::IsSelected()
{
	return selectedObject != nullptr;
}

void UIManager::SetCallbackToUI(std::string label, unique_ptr<Callback> callback)
{
	if (uimap.find(label) != uimap.end())
	{
		uimap[label]->callback = std::move(callback);
	}
}

void UILinkNode::Insert(UILinkNode* linkNode)
{
	if (next != nullptr)
	{
		next->prev = linkNode;
		linkNode->next = next;
		linkNode->prev = this;
	}

	this->next = linkNode;
}

UILinkNode* UILinkNode::Erase(UILinkNode* linkNode)
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

void UILinkNode::Render()
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
