#include "stdafx.h"
#include <imgui.h>
#include "UIGizmo.h"
#include <ImGuizmo.h>
#include "UIManager.h"
#include "Mesh.h"
#include "Renderer.h"

ImGuizmo::OPERATION ConvertToImGuizmoOperation(UIGizmo::eUIGizmoOperation operation)
{
	switch (operation)
	{
	case UIGizmo::Translation:
		return ImGuizmo::TRANSLATE;
	case UIGizmo::Rotation:
		return ImGuizmo::ROTATE;
	case UIGizmo::Scale:
		return ImGuizmo::SCALE;
	}

}

void UIGizmo::Render()
{
	if (UIManager::g_uiManager->IsSelected() == true)
	{
		auto& camera = UIManager::g_uiManager->renderer->camera;
		Matrix view = camera.GetView();
		Matrix proj = camera.GetProjection();
		Matrix world = UIManager::g_uiManager->selectedObject->GetTransformRef()->GetWorldMatrix();
		ImGuizmo::DrawCubes(&view._11, &proj._11, &world._11, 1);

		Mesh* obj = UIManager::g_uiManager->selectedObject;

		Transform* trans = obj->GetTransformRef();
		ImGui::InputFloat3("Tr", &trans->translation.x);
		ImGui::InputFloat3("Rt", &trans->rotation.x);
		ImGui::InputFloat3("St", &trans->rotation.x);

		if (ImGui::IsKeyPressed(ImGuiKey_T) == true)
		{
			operation = UIGizmo::eUIGizmoOperation::Translation;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_R) == true)
		{
			operation = UIGizmo::eUIGizmoOperation::Rotation;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_S) == true)
		{
			operation = UIGizmo::eUIGizmoOperation::Scale;
		}

	}
	
}
