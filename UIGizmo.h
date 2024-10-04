#pragma once

#include "UI.h"

class UIGizmo : UI
{
public:
	enum eUIGizmoOperation
	{
		Translation = 0,
		Rotation = 1,
		Scale = 2
	};

	enum eUIGizmoMode
	{
		Local = 0,
		World = 1,
	};
public:
	UIGizmo() {}

	virtual void Render() override;

	eUIGizmoOperation operation = eUIGizmoOperation::Translation;
	eUIGizmoMode mode = eUIGizmoMode::World;

};

