#include "stdafx.h"
#include "GizmoController.h"
#include "imgui.h"
#include <ImSequencer.h>
#include "Mesh.h"
#include "Renderer.h"
#include "Level.h"
#include "ServiceLoactor.h"
#include "SceneDisplayer.h"
void GizmoController::Render()
{
	static Matrix deltaMatrix;
	static float snap = 0.0f;

	if (model.has_value() == false)
	{
		return;
	}
	Mesh* mesh = std::any_cast<Mesh*>(model);
	auto renderer = ServiceLoactor::GetService<Renderer>();

	bool bDrawCube = false;

	ImGui::Begin("Property");
	if (mesh != nullptr)
	{
		{
			if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL))
				mode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD))
				mode = ImGuizmo::WORLD;
		}
		Transform* trans = mesh->GetTransformRef();
		ImGui::InputFloat3("Tranlate", &trans->position.x);
		ImGui::InputFloat3("Rotation", &trans->rotation.x);
		ImGui::InputFloat3("Scale", &trans->scale.x);

		ImGui::Checkbox("Draw Cube", &needDrawCube);
	}
	ImGui::End();

	if (mesh != nullptr)
	{

		auto& camera = renderer->camera;
		Matrix view = camera.GetView();
		Matrix proj = camera.GetProjection();

		Matrix world = mesh->GetTransformRef()->GetWorldMatrix();


		//if (mCurrentGizmoOperation != ImGuizmo::SCALE)
		/*{
			if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL))
				mode = ImGuizmo::LOCAL;
			ImGui::SameLine();
			if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD))
				mode = ImGuizmo::WORLD;
		}*/

		//ImGui::InputFloat3("Before", &world._41);
		ImGuiViewport* viewport = ImGui::GetWindowViewport();

		auto io = ImGui::GetIO();
		ImVec2 winPos = ImGui::GetWindowPos();
		ImVec2 DisplaySize = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
		ImGuizmo::SetRect(winPos.x, winPos.y, DisplaySize.x, DisplaySize.y);
		//ImGuizmo::SetRect(viewport->Pos.x, viewport->Pos.x, viewport->Size.x, viewport->Size.y);

		if(needDrawCube)
			ImGuizmo::DrawCubes(&view._11, &proj._11, &world._11, 1);


		Matrix after = world;
		ImGuizmo::Manipulate(&view._11, &proj._11, operation, mode, &after._11,
			&deltaMatrix._11
			, &snap);

		if (ImGuizmo::IsOver(ImGuizmo::OPERATION::TRANSLATE))
		{
			std::cout << "isOver" << endl;
		}

		//ImGui::InputFloat3("After", &after._41);


		mesh->SetWorldMatrix(after);

		Transform* trans = mesh->GetTransformRef();
		//ImGui::Text("snap %f", snap);
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

void GizmoController::SetDisaplyer(SceneDisplayer* dispalyer)
{
	display = dispalyer;
}

void GizmoController::SetMesh(Mesh* mesh)
{
	model = mesh;
}

