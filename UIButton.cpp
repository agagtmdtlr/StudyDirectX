#include "stdafx.h"
#include "UIButton.h"
#include "imgui.h"
#include <imgui.h>

void UIButton::Render()
{
	ImGui::Columns(3);

	if (ImGui::Button(GetLabel().c_str()) == true)
	{
		Action();
	}
}

void UIButton::Action()
{

}

std::string UIButton::GetLabel()
{


	switch (type)
	{
	case eUIButtonType::CreateMesh:
		return "CreateMesh";
	case eUIButtonType::DeleteMesh:
		return "DeleteMesh";
	case eUIButtonType::ImportMehs:
		return "ImportMesh";
	}

	return "";
}
