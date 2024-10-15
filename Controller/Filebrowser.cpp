#include "stdafx.h"
#include "Filebrowser.h"
#include <imgui.h>
#include "FileManager.h"

namespace fs = std::filesystem;


FileBrowser::FileBrowser()
{
	rootPath = "../Resources";
	rootPath = fs::absolute(rootPath);
	curpath = "../Resources";
	curpath = fs::absolute(curpath);
	Move(curpath);

	folderIco = TextureManager::RequestTexture(L"icon/folder.png");
	fileIco = TextureManager::RequestTexture(L"icon/file.png");
	imgIco = TextureManager::RequestTexture(L"icon/image.png");

}

void FileBrowser::Render()
{
	bool moveDir = false;
	fs::path movePth;
	if (ImGui::Begin("Content Browser"))
	{

		ImGui::Text(curpath.generic_string().c_str());


		static float padding = 16.0f;
		static float thumbnailSize = 64;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;


		int coumnCount = (int)(panelWidth / cellSize);
		if(coumnCount < 1) coumnCount = 1;
		ImGui::Columns(coumnCount,0,false);

		for (UINT i = 0; i < tokens.size(); i++)
		{
			bool clicked = false;
			bool dlicked = false;

			if (entrys[i].is_directory())
			{
				ImGui::ImageButton((ImTextureID)(folderIco->GetSRV()), { thumbnailSize,thumbnailSize });
				dlicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
				if(dlicked)
				{
					movePth = entrys[i].path();
					moveDir = true;
				}

			}
			else
			{
				std::filesystem::path pth = entrys[i].path();

				bool isImg = TextureManager::IsImage(pth);
				ImTextureID txId = isImg ? imgIco->GetSRV() : fileIco->GetSRV();
				ImGui::ImageButton(txId, { thumbnailSize,thumbnailSize });

				clicked = ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left);
				ImGuiDragDropFlags src_flags = 0;
				src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     //
				src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; //

				if (ImGui::BeginDragDropSource(src_flags))
				{
					void* data = nullptr;
					size_t dataSize = 0;
					//ImGuiCond cond;
					ImGui::SetDragDropPayload("ITEM", data, dataSize);

					ImGui::ImageButton("drag", txId, { thumbnailSize,thumbnailSize });
					ImGui::EndDragDropSource();
				}


				if (ImGui::BeginDragDropTarget())
				{
					const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ITEM");

					ImGui::EndDragDropTarget();
				}
			}

			


			ImGui::Text(tokens[i].c_str());

			
			ImGui::NextColumn();
		}

		ImGui::Columns(1);

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 128);
		// TODO: status bar
	}

	ImGui::End();
	


	if(moveDir == true)
		Move(movePth);

}

void FileBrowser::Move(std::filesystem::path pth)
{
	curpath = pth;
	tokens.clear();
	entrys.clear();

	if (curpath != rootPath)
	{
		entrys.push_back(fs::directory_entry(curpath.parent_path()));
		tokens.push_back("...");
	}

	
	fs::directory_iterator itr(pth);


	while (itr != fs::end(itr))
	{
		const fs::directory_entry entry = *itr;
		entrys.push_back(entry);

		string str;
		str += entry.path().filename().generic_string();

		tokens.push_back(str);
		
		itr++;
	}
}
