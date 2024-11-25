#include "stdafx.h"
#include "Filebrowser.h"
#include "imgui.h"
#include "FileManager.h"
#include "imgui_internal.h"
#include <commdlg.h>
#include "System.h"
#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <sstream>
#include <fstream>
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_ext.hpp>
#include <rapidxml/rapidxml_print.hpp>
namespace fs = std::filesystem;
using namespace rapidxml;

FileBrowser::FileBrowser()
{
	rootPath = "./Content";
	rootPath = fs::absolute(rootPath);

	if (fs::exists(rootPath) == false)
	{
		fs::create_directory(rootPath);
	}

	curpath = "./Content";
	curpath = fs::absolute(curpath);
	Move(curpath);

	{
		fs::path ipath;
		ipath = curpath;
		ipath += L"/icon/folder.png";
		folderIco = TextureManager::RequestTexture(ipath);

		ipath = curpath;
		ipath += L"/icon/file.png";
		fileIco = TextureManager::RequestTexture(ipath);

		ipath = curpath;
		ipath += L"/icon/image.png";
		imgIco = TextureManager::RequestTexture(ipath);

		ipath = curpath;
		ipath += L"/icon/mesh.png";
		meshIco = TextureManager::RequestTexture(ipath);

		ipath = curpath;
		ipath += L"/back.jpg";
		test = TextureManager::RequestTexture(ipath);
	}
	

}

void FileBrowser::Render()
{
	bool moveDir = false;
	fs::path movePth;
	static std::filesystem::path dragPath;
	ImVec2 imSize = ImGui::GetMainViewport()->Size;

	ImGuiWindowFlags wndFlag = ImGuiWindowFlags_MenuBar;

	ImGuiDragDropFlags dndFlag = ImGuiDragDropFlags_None;
	//dndFlag |= ImGuiDragDropFlags_SourceAllowNullID;
	//dndFlag |= ImGuiDragDropFlags_SourceExtern;
	if (ImGui::Begin("Content Browser", 0, wndFlag))
	{
		if (ImGui::Button("Import"))
		{
			ImportFile();
		}
		ImGui::Text(curpath.generic_string().c_str());

		ImGuiID parentID = ImGui::GetCurrentWindow()->ID;
		ImGuiChildFlags childFlag;
		//ImGui::BeginChild(parentID,ImVec2(0,0));

		//&& ImGui::IsMouseDragging(ImGuiMouseButton_Left) == true

		static float padding = 16.0f;
		static float thumbnailSize = 64;
		float cellSize = thumbnailSize + padding;
		float panelWidth = ImGui::GetContentRegionAvail().x;


		int coumnCount = (int)(panelWidth / cellSize);
		if (coumnCount < 1) coumnCount = 1;
		ImGui::Columns(coumnCount, 0, false);

		for (UINT i = 0; i < tokens.size(); i++)
		{

			bool clicked = false;
			bool dlicked = false;
			std::filesystem::path pth = entrys[i].path();
			bool isImg = TextureManager::IsImage(pth);
			bool isMesh = FileManager::IsMesh(pth);

			ImTextureID txId;

			if (entrys[i].is_directory())
			{
				txId = folderIco->GetSRV();
			}
			else if (isImg)
			{
				txId = imgIco->GetSRV();
			}
			else if (isMesh)
			{
				txId = meshIco->GetSRV();
			}
			else
			{
				txId = fileIco->GetSRV();
			}

			if (entrys[i].is_directory())
			{
				dlicked = ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left);
				if (dlicked)
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

				if (ImGui::IsItemHovered())
				{
					ImGui::BeginTooltip();
					ImGui::SetTooltip(tokens[i].c_str());
					ImGui::EndTooltip();
				}


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
		//ImGui::EndChild();



	}
	ImGui::End();





	if (moveDir == true)
		Move(movePth);

}

void FileBrowser::ImportMaterial(const aiScene* scene, std::filesystem::path fpath)
{
	rapidxml::xml_document<> doc;
	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);

	std::filesystem::path fname = fpath.filename();
	fname.replace_extension();
	std::filesystem::path dirpath = fpath.parent_path();

	// root node
	xml_node<>* root = doc.allocate_node(node_element, "material");
	root->append_attribute(doc.allocate_attribute("version", "1.0"));
	root->append_attribute(doc.allocate_attribute("type", "example"));
	doc.append_node(root);

	for (UINT m = 0; m < scene->mNumMaterials; m++)
	{
		aiMaterial* material = scene->mMaterials[m];

		xml_node<>* child = doc.allocate_node(node_element, doc.allocate_string(material->GetName().C_Str()));
		root->append_node(child);

		
		std::cout << material->GetName().C_Str() << endl;

		aiString diffuse;
		if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, 0, &diffuse)) {}
		else if (AI_SUCCESS == material->GetTexture(aiTextureType_BASE_COLOR, 0, &diffuse)) {}
		//std::cout << "diffuse " << diffuse.C_Str() << endl;

		aiString normal;
		if (AI_SUCCESS == material->GetTexture(aiTextureType_HEIGHT, 0, &normal)) {}
		else if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMALS, 0, &normal)) {}
		else if (AI_SUCCESS == material->GetTexture(aiTextureType_NORMAL_CAMERA, 0, &normal)) {}

		string diffusefullpath = dirpath.string();
		diffusefullpath += "\\";
		diffusefullpath += diffuse.C_Str();

		xml_node<>* diff = doc.allocate_node(node_element, "diffuse", doc.allocate_string(diffusefullpath.c_str()));
		child->append_node(diff);
		std::cout << "diffuse " << diffuse.C_Str() << endl;

		string normalfullpath = dirpath.string();
		normalfullpath += "\\";
		normalfullpath += normal.C_Str();

		xml_node<>* nor = doc.allocate_node(node_element, "normal", doc.allocate_string(normalfullpath.c_str()));
		child->append_node(nor);
		std::cout << "normal " << normal.C_Str() << endl;

	} // materials

	std::string xml_as_string;
	rapidxml::print(std::back_inserter(xml_as_string), doc);

	ofstream materialOut;

	std::filesystem::path mpath(curpath);
	mpath += "\\";
	mpath += fname.string();
	mpath += ".material";
	materialOut.open(mpath.c_str());
	materialOut << xml_as_string;
	materialOut.clear();
	materialOut.close();
}

void FileBrowser::ImportMesh(const aiScene* scene, std::filesystem::path fname)
{
	AssetHeader header;
	header.version = 0;
	header.type = 1; // static;

	std::string outputpath = curpath.string();
	outputpath += "\\";
	outputpath += fname.string();
	outputpath += ".asset";

	std::ofstream outputFile(outputpath.c_str(), std::ios::binary);
	if (!outputFile.is_open()) {
		std::cerr << "파일을 열 수 없습니다." << std::endl;
		return;
	}
	UINT mSize = scene->mNumMeshes;
	outputFile.write(reinterpret_cast<char*>(&mSize), sizeof(UINT));

	vector<UINT> materialIndexPerMesh(scene->mNumMeshes);

	for (unsigned int m = 0; m < scene->mNumMeshes; m++)
	{
		aiMesh* aimesh = scene->mMeshes[m];
		vector<Vertex> vertices(aimesh->mNumVertices);

		aiVector3D* aivertices = aimesh->mVertices;
		aiVector3D* ainormals = aimesh->mNormals;
		aiVector3D* aiuvs = nullptr;
		if (aimesh->HasTextureCoords(0))
			aiuvs = aimesh->mTextureCoords[0];

		size_t off = 0;

		for (Vertex& v : vertices)
		{
			memcpy(&v.position.x, aivertices, sizeof(Vector3));
			memcpy(&v.normal.x, ainormals, sizeof(Vector3));
			if (aiuvs != nullptr)
				memcpy(&v.uv, aiuvs, sizeof(Vector2));
			off++;
			if (off <= vertices.size())
			{
				aivertices++;
				ainormals++;
				aiuvs++;
			}

		}

		vector<UINT> indices(aimesh->mNumFaces * 3);
		for (UINT i = 0; i < aimesh->mNumFaces; i++)
		{
			memcpy(indices.data() + (i * 3), aimesh->mFaces[i].mIndices, sizeof(UINT) * 3);
		}

		UINT vSize = vertices.size();
		UINT vdatasize = sizeof(Vertex) * vSize;
		UINT iSize = indices.size();
		UINT idatasize = sizeof(UINT) * iSize;

		//outputFile.write(reinterpret_cast<char*>(&aimesh->mMaterialIndex), sizeof(UINT));
		outputFile.write(reinterpret_cast<char*>(&vSize), sizeof(UINT));
		outputFile.write(reinterpret_cast<char*>(vertices.data()), sizeof(Vertex) * vSize);
		outputFile.write(reinterpret_cast<char*>(&iSize), sizeof(UINT));
		outputFile.write(reinterpret_cast<char*>(indices.data()), sizeof(UINT) * iSize);
	}
	outputFile.close();
}


//TODO :: move to filemanager
void FileBrowser::ImportFile()
{
	OPENFILENAME OFN;
	TCHAR filePathName[100] = L"";
	TCHAR lpstrFile[100] = L"";
	static TCHAR filter[] = L"모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";

	memset(&OFN, 0, sizeof(OPENFILENAME));
	OFN.lStructSize = sizeof(OPENFILENAME);
	OFN.hwndOwner = System::systemHandle->GetWindowHandle();
	OFN.lpstrFilter = filter;
	OFN.lpstrFile = lpstrFile;
	OFN.nMaxFile = 100;
	OFN.lpstrInitialDir = L".";

	if (GetOpenFileName(&OFN) != 0) {
		//wsprintf(filePathName, L"%s 파일을 열겠습니까?", OFN.lpstrFile);
		//int id = MessageBox(System::systemHandle->GetWindowHandle(), filePathName, L"열기 선택", MB_OK | MB_OKCANCEL);
		//if (id == IDOK)
		{
			wstring ws(OFN.lpstrFile);
			string path;
			path.assign(ws.begin(), ws.end());
			Assimp::Importer importer;
			std::filesystem::path fpath(OFN.lpstrFile);
			std::filesystem::path fname = fpath.filename();
			fname.replace_extension();
			std::filesystem::path fextension = fpath.extension();

			std::string extBuffer;
			importer.GetExtensionList(extBuffer);
			const char delimiter = ';';
			std::istringstream iss(extBuffer);
			std::vector<string> result;
			std::string token;
			while (std::getline(iss, token, delimiter))
			{
				result.push_back(token);
			}

			if (importer.IsExtensionSupported(fextension.string()))
			{
				Assimp::Importer importer;

				int flag = aiProcess_Triangulate;
				flag |= aiProcess_GenUVCoords;

				const aiScene* scene = importer.ReadFile(fpath.string(), flag);

				if (scene == nullptr)
				{
					std::cout << importer.GetErrorString() << std::endl;;
					return;
				}

				

				if(scene->HasMaterials())
				{
					ImportMaterial(scene, fpath);
				}

				if (scene->hasSkeletons())
				{
					// is dynamic mesh;
				}

				if (scene->HasMeshes())
				{
					ImportMesh(scene,fname);
				}
				
			}

		}
	}

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
