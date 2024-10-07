#include "stdafx.h"
#include "Input.h"
#include <imgui.h>

Input::Input()
{
}

Input::~Input()
{
}

void Input::Initialize()
{
    int i;
    for (i = 0; i < 256; i++)
    {
        keys[i] = false;
    }
}

void Input::Event()
{
    auto io = ImGui::GetIO();
    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        ImVec2 pos = ImGui::GetMousePos();
    }


}

void Input::KeyDown(unsigned int input)
{
    keys[input] = true;
}

void Input::KeyUp(unsigned int input)
{
    keys[input] = false;
}

bool Input::IsKeyDown(unsigned int input)
{
    return keys[input];
}
