#include "stdafx.h"
#include "ButtonController.h"
#include "imgui.h"
#include <imgui.h>

void ButtonController::Render()
{
	ImGui::Columns(3);

	if (ImGui::Button(label.c_str()) == true)
	{
		Action();
	}
}

void ButtonController::Action()
{
	(*callback)();
}

