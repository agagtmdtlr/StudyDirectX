#include "stdafx.h"
#include "UIButton.h"
#include "imgui.h"
#include <imgui.h>

void UIButton::Render()
{
	ImGui::Columns(3);

	if (ImGui::Button(label.c_str()) == true)
	{
		Action();
	}
}

void UIButton::Action()
{
	(*callback)();
}

