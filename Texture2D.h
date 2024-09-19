#pragma once
#include "Texture.h"
#include "IImageValidator.h"
#include "IImageLoader.h"

class Texture2DImageLoader : public IImageLoader
{
	virtual void LoadImageFromFile(std::string filename);
};

class Texture
{
	virtual void Initialize();
};