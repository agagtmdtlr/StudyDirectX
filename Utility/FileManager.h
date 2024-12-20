#pragma once
#include "Texture.h"
#include <unordered_map>
#include <filesystem>
class FileManager
{
public:
	static unsigned char* LoadImage(const char* filename, int* width, int* height, int* channels);
	static void WriteImage(const char* filename, int width, int height, int channels, const void* data);
	static bool IsMesh(std::wstring file);

	static shared_ptr<Texture> CreateTextureFromFile(const std::wstring& filename);

};

class TextureManager
{
public:
	static shared_ptr<Texture> RequestTexture(std::wstring filename);
	static shared_ptr<Texture> RequestTexture(std::string filename);

	static bool ConvertImageToDDS(std::wstring src, std::wstring dest);
	static bool IsImage(std::wstring file);

	using TextureMap = std::unordered_map<std::wstring, shared_ptr<Texture>>;
private:
	static TextureManager* Get();
	bool IsExistedTexture(const std::wstring& filename) const;
	bool CreateTextureFromFile(const std::wstring& filename);
	shared_ptr<Texture> GetTexture(const std::wstring& filename);

	TextureMap textureMap;
};

class MeshFileManager
{
public:
	void RequestMesh(std::wstring filename);
};
