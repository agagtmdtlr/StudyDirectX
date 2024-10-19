#include "stdafx.h"
#include "GizmoController.h"
#include "imgui.h"
#include <ImSequencer.h>
#include "Mesh.h"
#include "Renderer.h"



void GizmoController::Render()
{
	static Matrix deltaMatrix;
	static float snap = 0.0f;

	if (model.has_value() == false )
	{
		return;
	}
	Mesh* mesh = std::any_cast<Mesh*>(model);

	if (mesh != nullptr)
	{
		auto& camera = Renderer::g_renderer->camera;
		Matrix view = camera.GetView();
		Matrix proj = camera.GetProjection();

		Matrix world = mesh->GetTransformRef()->GetWorldMatrix();


		//if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		{
			if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL))
				mode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD))
				mode = ImGuizmo::WORLD;
		}

		ImGui::InputFloat3("Before", &world._41);
		Matrix after = world;

		auto io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		//ImGuizmo::DrawCubes(&view._11, &proj._11, &world._11, 1);
		ImGuizmo::Manipulate(&view._11, &proj._11, operation, mode, &after._11,
			&deltaMatrix._11
			, &snap);

		ImGui::InputFloat3("After", &after._41);


		mesh->SetWorldMatrix(after);

		Transform* trans = mesh->GetTransformRef();
		ImGui::SliderFloat3("Tr", &trans->position.x, -180, 180);
		ImGui::SliderFloat3("Rt", &trans->rotation.x, -180, 180);
		ImGui::SliderFloat3("St", &trans->scale.x, -180, 180);
		ImGui::Text("snap %f", snap);
		if (ImGui::IsKeyPressed(ImGuiKey_T) == true)
		{
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_R) == true)
		{
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_S) == true)
		{
		}

	}

}

