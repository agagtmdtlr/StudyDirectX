#include "Texture.h"
#include "FileManager.h"

namespace slab
{
	Texture::Texture(const std::string& filename)
	{
		unsigned char* img = FileManager::LoadImage(filename.c_str(), &width, &height, &channels);

		image.resize(width * height * channels);
		memcpy(image.data(), img, image.size() * sizeof(uint8_t));

		delete[] img;
	}

	Texture::Texture(const int& width, const int& height, const std::vector<vec3>& pixels)
		: width(width), height(height), channels(3)
	{
		image.resize(width * height* channels);

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				const auto& color = pixels[i + j * width];
				
				image[(i + j * width) * channels + 0] = uint8_t(color.r * 255);
				image[(i + j * width) * channels + 1] = uint8_t(color.g * 255);
				image[(i + j * width) * channels + 2] = uint8_t(color.b * 255);
			}
		}
	}

}