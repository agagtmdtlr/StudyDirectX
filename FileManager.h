#pragma once
#include "Texture2D.h"
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
	static ComPtr<ID3D11Texture2D> RequestTexture(std::wstring filename);
	static bool ConvertImageToDDS(std::wstring src, std::wstring dest);

	using TextureMap = std::unordered_map<std::wstring, ComPtr<ID3D11Texture2D>>;
private:
	static TextureManager* Get();
	bool IsExistedTexture(const std::wstring& filename) const;
	bool CreateTextureFromFile(const std::wstring& filename);
	ComPtr<ID3D11Texture2D> GetTexture(const std::wstring& filename);

	TextureMap textureMap;
};


