#include "stdafx.h"
#include "FileManager.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <DirectXTex.h>
#include "Path.h"


using namespace DirectX;

unsigned char* FileManager::LoadImage(const char* filename, int* width, int* height, int* channels)
{
	unsigned char* img = stbi_load(filename, width, height, channels, 0);

    if (*width)
    {   
        std::cout << "Success : " << filename << " " << *width << " " << *height << " " << *channels << std::endl;
        
    }
    else
    {
        std::cout << "Error : reading " << filename << " failed." << std::endl;
    }

	return img;
}

void FileManager::WriteImage(const char* filename, int width, int height, int channels, const void* data)
{
    stbi_write_png(filename, width, height, channels, data, width * channels);
}

bool FileManager::IsMesh(std::wstring file)
{
    wstring ext = Path::GetExtension(file);
    return ext == L"fbx" || ext == L"obj";
}

shared_ptr<Texture>  FileManager::CreateTextureFromFile(const std::wstring& filename)
{
    shared_ptr<Texture> texture;
    wstring ext = Path::GetExtension(filename);

    TexMetadata metaData;
    ZeroMemory(&metaData, sizeof(metaData));
    ScratchImage scratchimage;

    std::wstring fullPth = L"../Resources/";
    fullPth += filename;
    HRESULT hr;
    if (ext == L"png" || ext == L"jpg")
    {
        //DirectX::GetMetadataFromWICFile(filename.c_str(), WIC_FLAGS::WIC_FLAGS_NONE , metaData );
        hr = DirectX::LoadFromWICFile(fullPth.c_str(), WIC_FLAGS_NONE, &metaData, scratchimage);
    }
    else if (ext == L"dds")
    {
        hr = DirectX::LoadFromDDSFile(fullPth.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, &metaData, scratchimage);
    }

    if (FAILED(hr))
    {
        std::wcout << "faile texture load " << filename << std::endl;
    }

    auto device = D3D::GetDevice();
    auto dc = D3D::GetDC();



    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.Width = metaData.width;
    desc.Height = metaData.height;
    desc.MipLevels = metaData.mipLevels;
    desc.ArraySize = metaData.arraySize;
    desc.Format = metaData.format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 1;

    ComPtr<ID3D11ShaderResourceView> srv;

    DirectX::CreateShaderResourceViewEx(
        device,
        scratchimage.GetImages(),
        scratchimage.GetImageCount(),
        metaData,
        D3D11_USAGE_DEFAULT,
        D3D11_BIND_SHADER_RESOURCE,
        0, 0, CREATETEX_DEFAULT, srv.GetAddressOf());

    texture = make_shared<Texture>(metaData, srv.Get());

    return texture;
}

shared_ptr<Texture> TextureManager::RequestTexture(std::wstring filename)
{
    TextureManager* manager = TextureManager::Get();
    shared_ptr<Texture> texture;
    if (manager->IsExistedTexture(filename) ==  false)
    {
        manager->CreateTextureFromFile(filename);
    }

    return manager->GetTexture(filename);
}

bool TextureManager::ConvertImageToDDS(std::wstring src, std::wstring dest)
{
    return false;
}

bool TextureManager::IsImage(std::wstring file)
{
    wstring ext =  Path::GetExtension(file);
    return ext == L"png" || ext == L"jpg" || ext == L"dds";
}

TextureManager* TextureManager::Get()
{
    static TextureManager* manager = nullptr;
    if (manager == nullptr)
    {
        manager = new TextureManager();
    }
    return manager;
}

bool TextureManager::IsExistedTexture(const std::wstring& filename) const
{
    return textureMap.find(filename) != textureMap.end();
}

bool TextureManager::CreateTextureFromFile(const std::wstring& filename)
{

    wstring ext = Path::GetExtension(filename);

    TexMetadata metaData;
    ZeroMemory(&metaData, sizeof(metaData));
    ScratchImage scratchimage;

    std::wstring fullPth = L"../Resources/";
    fullPth += filename;
    HRESULT hr;
    if (ext == L"png" || ext == L"jpg")
    {
        //DirectX::GetMetadataFromWICFile(filename.c_str(), WIC_FLAGS::WIC_FLAGS_NONE , metaData );
        hr = DirectX::LoadFromWICFile(fullPth.c_str(),WIC_FLAGS_NONE , &metaData, scratchimage);
    }
    else if (ext == L"dds")
    {
        hr = DirectX::LoadFromDDSFile(fullPth.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, &metaData, scratchimage);
    }

    if (FAILED(hr))
    {
        std::wcout << "faile texture load " << filename << std::endl;
    }

    auto device = D3D::GetDevice();
    auto dc = D3D::GetDC();

    

    D3D11_TEXTURE2D_DESC desc;
    ZeroMemory(&desc,sizeof(desc));
    desc.Width = metaData.width;
    desc.Height = metaData.height;
    desc.MipLevels = metaData.mipLevels;
    desc.ArraySize = metaData.arraySize; 
    desc.Format = metaData.format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 1;

    ComPtr<ID3D11ShaderResourceView> srv;

    DirectX::CreateShaderResourceViewEx(
    device, 
    scratchimage.GetImages(), 
    scratchimage.GetImageCount(), 
    metaData, 
    D3D11_USAGE_DEFAULT, 
    D3D11_BIND_SHADER_RESOURCE, 
    0, 0, CREATETEX_DEFAULT, srv.GetAddressOf());

    textureMap[filename] = make_shared<Texture>(metaData, srv.Get());

    return true;
}

shared_ptr<Texture> TextureManager::GetTexture(const std::wstring& filename)
{
    return textureMap[filename];
}

void MeshFileManager::RequestMesh(std::wstring filename)
{
}
