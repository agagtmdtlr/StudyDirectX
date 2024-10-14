#include "stdafx.h"
#include "Button.h"
#include "imgui.h"
#include <imgui.h>

void Button::Render()
{
	if (ImGui::Button(label.c_str()) == true)
	{
		Action();
	}
}

void Button::Action()
{
	(*callback)();
}

