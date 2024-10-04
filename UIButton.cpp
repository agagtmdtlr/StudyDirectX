#include "stdafx.h"
#include "UIButton.h"
#include "imgui.h"

void UIButton::Render()
{
	if (ImGui::Button(GetLabel().c_str(), ImVec2(50, 10)) == true)
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
