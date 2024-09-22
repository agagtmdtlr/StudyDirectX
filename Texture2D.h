#pragma once
#include "IImageValidator.h"
#include "IImageLoader.h"

class Texture2DImageLoader : public IImageLoader
{
	virtual void LoadImageFromFile(std::string filename);
};

class Texture2D
{
public:
	Texture2D(std::string filename);

private:
	std::string name;

};