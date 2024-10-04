#include "stdafx.h"
#include "UIManager.h"
#include "imgui.h"
#include "Level.h"
#include "Sphere.h"
#include "UIButton.h"
#include "Mesh.h"

UIManager* UIManager::g_uiManager = nullptr;

UIManager::UIManager(SystemClass* system, Renderer* renderer)
	:system(system), renderer(renderer), header(nullptr), selectedObject(nullptr)
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
		btn->type = eUIButtonType::CreateMesh;
		header->Insert(add);
	}
	{
		UILinkNode* del = new UILinkNode();
		del->ui = make_unique<UIButton>();
		UIButton* btn = dynamic_cast<UIButton*>(del->ui.get());
		btn->type = eUIButtonType::DeleteMesh;
		header->Insert(del);
	}
}

void UIManager::Render()
{
	bool addSphere = ImGui::Button("Add Sphere", ImVec2(50, 10));


	if (addSphere)
	{
	}

	for (auto& ui : uis)
	{
		ui->Render();
	}
}

void UIManager::CreateSphere()
{
}

bool UIManager::IsSelected()
{
	return selectedObject != nullptr;
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
