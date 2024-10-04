#pragma once
#include <functional>
#include <imgui.h>

class UI
{
public:
	UI() {}

	virtual ~UI() {}

	virtual void Render() = 0;
};





