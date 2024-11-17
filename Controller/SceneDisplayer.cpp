#include "stdafx.h"
#include "SceneDisplayer.h"
#include <imgui.h>
#include <imgui_internal.h>
#include "D3D.h"
#include "FileManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <algorithm>
#include "RenderCommon.h"
#include "PrimitiveBuffer.h"
#include "Renderer.h"
#include "Level.h"
#include "ServiceLoactor.h"

void SceneDisplayer::Render()
{
	bool bopen = true;
	ImGuiWindowFlags wflag = ImGuiWindowFlags_None;
	//wflag |= ImGuiWindowFlags_NoBackground;
	ImGuiDragDropFlags dndFlag = ImGuiDragDropFlags_None;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });

	ImVec2 newSize;
	Renderer* renderer = ServiceLoactor::GetService<Renderer>();
	Level* level = ServiceLoactor::GetService<Level>();
	
	//ImGui::Begin("viewport", 0, wflag);
	ImGui::Begin("viewport",0, wflag);
	{
		ImGuizmo::SetDrawlist(ImGui::GetWindowDrawList());
		ImTextureID txid = (ImTextureID)(D3D::GetSRV());

		ImVec2 sz = ImGui::GetWindowContentRegionMax() - ImGui::GetWindowContentRegionMin();
		//ImVec2 sz = ImGui::GetContentRegionAvail();
		D3D::SetDisplaySize(sz.x,sz.y);
		 
		ImVec2 pos = ImGui::GetMousePos()-ImGui::GetWindowPos();
		if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			Ray ray = renderer->camera.ScreenPointToRay(Vector2(pos.x, pos.y));
			Mesh* selectedMesh = level->RayCast(ray);
			if(selectedMesh != nullptr)
				gizmo.SetMesh(selectedMesh);
		}

		if (ImGui::IsKeyPressed(ImGuiKey_Escape) == true)
		{
			gizmo.SetMesh(nullptr);
		}
		ImGui::Image(txid, ImGui::GetContentRegionMax() - ImVec2(0, 0));
		ImGui::PushID(100);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ITEM", dndFlag))
			{
				std::cout << ImGui::GetWindowContentRegionMax().x << " " << ImGui::GetWindowContentRegionMax().y << std::endl;;

				// if dropped item is mesh then spawn to scene
				LoadMesh();
				std::cout << "drop item" << std::endl;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();

		gizmo.Render();

	}
	ImGui::End();


	ImGui::PopStyleVar();
}

void SceneDisplayer::LoadMesh()
{
	
}
