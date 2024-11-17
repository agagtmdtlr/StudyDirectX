#pragma once
#include "GizmoController.h"
class SceneDisplayer
{
public:
	virtual void Render();

	void LoadMesh();

	GizmoController gizmo;
};

