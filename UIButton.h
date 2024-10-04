#pragma once
#include "UI.h"

enum class eUIButtonType
{
	CreateMesh = 0,
	DeleteMesh = 1,
	ImportMehs = 2,
};
class UIButton : public UI
{
public:
	virtual void Render() override final;
	virtual void Action();

	std::string GetLabel();
	eUIButtonType type;
	void *caller;
};

