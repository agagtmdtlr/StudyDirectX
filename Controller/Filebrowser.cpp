#include "stdafx.h"
#include "Filebrowser.h"
#include <imgui.h>

namespace fs = std::filesystem;


FileBrowser::FileBrowser()
{
	rootPath = "../Resources";
	rootPath = fs::absolute(rootPath);
	curpath = "../Resources";
	curpath = fs::absolute(curpath);
	Move(curpath);


}

void FileBrowser::Render()
{
	bool moveDir = false;
	fs::path movePth;
	if (ImGui::Begin("Content Browser"))
	{

		ImGui::Text(curpath.generic_string().c_str());


		static float padding = 16.0f;
		static float thumbnailSize = 128;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;

		int coumnCount = (int)(panelWidth / cellSize);
		if(coumnCount < 1) coumnCount = 1;
		ImGui::Columns(coumnCount,0,false);

		for (UINT i = 0; i < tokens.size(); i++)
		{
			bool clicked = ImGui::Button(tokens[i].c_str(), { thumbnailSize,thumbnailSize });
			clicked &= ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
			ImGui::Text(tokens[i].c_str());
			if (entrys[i].is_directory())
			{
				if(clicked)
				{
					movePth = entrys[i].path();
					moveDir = true;
				}
			}
			else
			{
				if (clicked)
				{
				}
			}
			
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
