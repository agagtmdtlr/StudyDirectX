#pragma once
#include "Texture.h"
#include <unordered_map>

class FileManager
{
public:
	static unsigned char* LoadImage(const char* filename, int* width, int* height, int* channels);
	static void WriteImage(const char* filename, int width, int height, int channels, const void* data);
};

class TextureManager
{
public:
	static shared_ptr<Texture> RequestTexture(std::wstring filename);
	static bool ConvertImageToDDS(std::wstring src, std::wstring dest);

	using TextureMap = std::unordered_map<std::wstring, shared_ptr<Texture>>;
private:
	static TextureManager* Get();
	bool IsExistedTexture(const std::wstring& filename) const;
	bool CreateTextureFromFile(const std::wstring& filename);
	shared_ptr<Texture> GetTexture(const std::wstring& filename);

	TextureMap textureMap;
};


