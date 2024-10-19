#include "stdafx.h"
#include "Filebrowser.h"
#include "imgui.h"
#include "FileManager.h"
#include "imgui_internal.h"
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
	test = TextureManager::RequestTexture(L"back.jpg");
}

void FileBrowser::Render()
{
	bool moveDir = false;
	fs::path movePth;
	static std::filesystem::path dragPath;
	ImVec2 imSize = ImGui::GetMainViewport()->Size;

	ImGuiDragDropFlags dndFlag = ImGuiDragDropFlags_None;
	//dndFlag |= ImGuiDragDropFlags_SourceAllowNullID;
	//dndFlag |= ImGuiDragDropFlags_SourceExtern;
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
			std::filesystem::path pth = entrys[i].path();
			bool isImg = TextureManager::IsImage(pth);
			ImTextureID txId = entrys[i].is_directory() ? (ImTextureID)(folderIco->GetSRV()) : 
			(isImg ? imgIco->GetSRV() : fileIco->GetSRV());

			if (entrys[i].is_directory())
			{
				dlicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
				if(dlicked)
				{
					movePth = entrys[i].path();
					moveDir = true;
				}

			}

			string lab = "content";
			lab += to_string(i);

			ImGui::PushID(i);
			{
				ImGui::ImageButton(txId, { thumbnailSize,thumbnailSize });
				clicked = ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left);

				
				static bool lastFrameWasAcceted = false;
				if (ImGui::BeginDragDropSource(dndFlag))
				{
					dragPath = pth;
					//ImGuiCond cond;
					ImGui::ImageButton("drag", txId, { thumbnailSize,thumbnailSize });

					void* data = &dragPath;
					size_t dataSize = sizeof(dragPath);
					lastFrameWasAcceted = ImGui::SetDragDropPayload("DND_ITEM", data, dataSize, ImGuiCond_None);
					ImGui::EndDragDropSource();
				}

			}
			ImGui::PopID();

			


			ImGui::Text(tokens[i].c_str());

			
			ImGui::NextColumn();
		}

		ImGui::Columns(1);	

		ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 128);
		// TODO: status bar
	}
	ImGui::End();


	bool bopen = true;
	ImGuiWindowFlags wflag;
	wflag |= ImGuiWindowFlags_NoInputs;
	ImGui::Begin("viewport");
	{
		//ImTextureID txid = (ImTextureID)(test->GetSRV());
		ImTextureID txid = (ImTextureID)(D3D::GetSRV());



		ImVec2 sz= ImGui::GetContentRegionAvail();
		ImGui::Image(txid, ImGui::GetContentRegionMax());
		ImGui::PushID(100);
		ImRect rect;
		rect.Min = ImVec2(0, 0);
		rect.Max = imSize;
		ImGui::GetCurrentWindow()->GetID(100);
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_ITEM", dndFlag))
			{
				std::cout << "drop item" << std::endl;
			}
			ImGui::EndDragDropTarget();
		}
		ImGui::PopID();
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
