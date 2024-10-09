#pragma once

#include "Controller.h"

#include <imgui.h>
#include <ImGuizmo.h>

class Mesh;
class GizmoController : public Controller
{

public:
	GizmoController() {}
	virtual ~GizmoController() override {}

	virtual void Render() override;
	void SetMesh(Mesh* mesh);

	ImGuizmo::OPERATION  operation = ImGuizmo::OPERATION::TRANSLATE;
	ImGuizmo::MODE mode = ImGuizmo::WORLD;

};

