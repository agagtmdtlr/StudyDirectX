#pragma once
#include <string>

class IImageLoader
{
public:
	IImageLoader() {}
	virtual ~IImageLoader() {}
	virtual void LoadImageFromFile(std::string filename) = 0;
};