#include "stdafx.h"
#include "UIManager.h"
#include "imgui.h"
#include "Level.h"

UIManager::UIManager(SystemClass* system)
	:system(system)
{	
}

void UIManager::Render()
{
	bool addSphere = ImGui::Button("Add Sphere", ImVec2(50,10));
	if (addSphere)
	{
	}

	for (auto& ui : uis)
	{
		ui->Render();
	}
}
