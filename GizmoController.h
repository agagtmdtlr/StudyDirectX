#pragma once

#include "Controller.h"

class GizmoController : public Controller
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
	GizmoController() {}

	virtual void Render() override;

	eUIGizmoOperation operation = eUIGizmoOperation::Translation;
	eUIGizmoMode mode = eUIGizmoMode::World;

};

