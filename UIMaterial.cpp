#include "stdafx.h"
#include "UIMaterial.h"
#include "imgui.h"

UIMaterial::UIMaterial(Material* material)
	:material(material)
{
}

void UIMaterial::Render()
{
	ImGui::BeginChild("material", ImVec2(200,200));
	ImGui::DragFloat("material diffuse", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("material specular", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
	ImGui::DragFloat("material ambient", &(material->desc.diff), 0.1f, 0.0f, 1.0f);
	ImGui::EndChild();
}
