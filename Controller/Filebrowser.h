#pragma once

#include "Controller.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include "Texture.h"
#include <oleidl.h>


class FileImporter
{
public:

};

struct AssetHeader
{
	UINT version;
	UINT type; // skinned or mesh
};

struct aiScene;
class  std::filesystem::path;
class FileBrowser : public Controller
{

public:
	FileBrowser();
	virtual void Render();

	void ImportMaterial(const aiScene* scene, std::filesystem::path fname);
	void ImportMesh(const aiScene* scene, std::filesystem::path fname);

	void ImportFile();

	void Move(std::filesystem::path pth);
	std::filesystem::path curpath;
	std::filesystem::path rootPath;

	std::vector<std::filesystem::directory_entry> entrys;
	std::vector<std::string> tokens;

	shared_ptr<Texture> folderIco;
	shared_ptr<Texture> fileIco;
	shared_ptr<Texture> imgIco;
	shared_ptr<Texture> meshIco;

	shared_ptr<Texture> test;


};

// create a clas inheriting from IDRopTarget
class DropManager :public IDropTarget
{
public:
	//--- implement the IUnknown parts
	// you could do this the proper way iwth InterlockedIncrement etc
	// but I've left out stuff that's not exactly necessary for brevity
	ULONG AddRef() { return 1; }
	ULONG Release() { return 0; }

	// we handle drop tharget let other kno
	HRESULT QueryInterface(REFIID riid, void** ppvObject)
	{
		if (riid == IID_IDropTarget)
		{
			*ppvObject = this;
			return S_OK;
		}
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	//--- implement the IDropTarget parts
	// occurs when we drag files into our applications view
	HRESULT DragEnter(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		// TODO: check whether we can handle this type of object at all and set *pdwEffect &= DROPEFFECT_NONE if not;
		// do something useful to flag to our application that files have been dragged from the OS into our application
		// trigger MouseDown for button 1 within ImGui
		//ImGui::IsMouseClicked()

		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}

	// occurs when we drag files out from our applications view
	HRESULT DragLeave() { return S_OK; }
	// occurs when we drag the mouse over our applications view whilst carrying files (post Enter, pre Leave)
	HRESULT DragOver(DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		// trigger MouseMove within ImGui, position is within pt.x and pt.y
		// grfKeyState contains flags for control, alt, shift etc
		//...

		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}

	// occurs when we release the mouse button to finish the drag-drop operation
	HRESULT Drop(IDataObject* pDataObj, DWORD grfKeyState, POINTL pt, DWORD* pdwEffect)
	{
		// grfKeyState contains flags for control, alt, shift etc

		// render the data into stgm using the data description in fmte
		FORMATETC fmte = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		STGMEDIUM stgm;

		if (SUCCEEDED(pDataObj->GetData(&fmte, &stgm)))
		{
			HDROP hdrop = (HDROP)stgm.hGlobal; // or reinterpret_cast<HDROP> if preferred
			UINT file_count = DragQueryFile(hdrop, 0xFFFFFFFF, NULL, 0);

			// we can drag more than one file at the same time, so we have to loop here
			for (UINT i = 0; i < file_count; i++)
			{
				TCHAR szFile[MAX_PATH];
				UINT cch = DragQueryFile(hdrop, i, szFile, MAX_PATH);
				if (cch > 0 && cch < MAX_PATH)
				{
					// szFile contains the full path to the file, do something useful with it
					// i.e. add it to a vector or something
				}
			}

			// we have to release the data when we're done with it
			ReleaseStgMedium(&stgm);

			// notify our application somehow that we've finished dragging the files (provide the data somehow)
			//...
		}

		// trigger MouseUp for button 1 within ImGui
		//...

		*pdwEffect &= DROPEFFECT_COPY;
		return S_OK;
	}
};
