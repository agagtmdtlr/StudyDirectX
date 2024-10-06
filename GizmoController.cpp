#include "stdafx.h"
#include "GizmoController.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include "ControllerManager.h"
#include "Mesh.h"
#include "Renderer.h"

ImGuizmo::OPERATION ConvertToImGuizmoOperation(GizmoController::eUIGizmoOperation operation)
{
	switch (operation)
	{
	case GizmoController::Translation:
		return ImGuizmo::TRANSLATE;
	case GizmoController::Rotation:
		return ImGuizmo::ROTATE;
	case GizmoController::Scale:
		return ImGuizmo::SCALE;
	}

}

void GizmoController::Render()
{
	if (ControllerManager::g_uiManager->IsSelected() == true)
	{
		auto& camera = ControllerManager::g_uiManager->renderer->camera;
		Matrix view = camera.GetView();
		Matrix proj = camera.GetProjection();
		Matrix world = ControllerManager::g_uiManager->selectedObject->GetTransformRef()->GetWorldMatrix();
		ImGuizmo::DrawCubes(&view._11, &proj._11, &world._11, 1);

		Mesh* obj = ControllerManager::g_uiManager->selectedObject;

		Transform* trans = obj->GetTransformRef();
		ImGui::InputFloat3("Tr", &trans->translation.x);
		ImGui::InputFloat3("Rt", &trans->rotation.x);
		ImGui::InputFloat3("St", &trans->rotation.x);

		if (ImGui::IsKeyPressed(ImGuiKey_T) == true)
		{
			operation = GizmoController::eUIGizmoOperation::Translation;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_R) == true)
		{
			operation = GizmoController::eUIGizmoOperation::Rotation;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_S) == true)
		{
			operation = GizmoController::eUIGizmoOperation::Scale;
		}

	}
	
}
