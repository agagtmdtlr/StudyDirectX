#include "stdafx.h"
#include "CameraController.h"
#include "Camera.h"
#include "Renderer.h"
#include "imgui.h"
#include "ServiceLoactor.h"
void CameraController::Render()
{
	/*if (model.has_value() == false)
	{
		return;
	}*/
	auto renderer = ServiceLoactor::GetService<Renderer>();
	auto& camera = renderer->camera;


	//Camera* camera = any_cast<Camera*>(model);
	if( ImGui::IsMouseDragging(ImGuiMouseButton_Right) )
	{
		ImVec2 drag = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);

		camera.rotation.y += drag.x * 0.005f;
		camera.rotation.x += drag.y * 0.005f;

		ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
	}

	float speed = 1.5f * accelaration;

	Vector3 dir,up;
	camera.GetDirectionAndUp(dir,up);

	Vector3 right = up.Cross(dir);
	right.Normalize();

	Vector3 move(0,0,0);

	if (ImGui::IsKeyDown(ImGuiKey_W))
	{
		move += dir * speed;
	}
	else if (ImGui::IsKeyDown(ImGuiKey_S))
	{
		move -= dir * speed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_A))
	{
		move -= right * speed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_D))
	{
		move += right * speed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_E))
	{
		move.y += speed;
	}
	if (ImGui::IsKeyDown(ImGuiKey_Q))
	{
		move.y -= speed;
	}

	camera.position += move;


	ImGui::Begin("Game Option");
	{
		ImGui::Text("Camera Position %f %f %f", camera.position.x , camera.position.y, camera.position.z);
		ImGui::Text("Camera Rotation %f %f %f", camera.rotation.x, camera.rotation.y, camera.rotation.z);

		ImGui::InputFloat("Camera Accelaraion", &accelaration);
	}
	ImGui::End();

}
