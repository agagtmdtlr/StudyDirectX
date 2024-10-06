#include "stdafx.h"
#include "MaterialController.h"
#include "Material.h"
#include "imgui.h"

MaterialController::MaterialController()
{
}

void MaterialController::Render()
{
	if (material != nullptr)
	{
		ImGui::BeginChild("material", ImVec2(200, 200));
		ImGui::DragFloat("material diffuse", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("material specular", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
		ImGui::DragFloat("material ambient", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
		ImGui::EndChild();
	}	
}
