#include "stdafx.h"
#include "FileManager.h"

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

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
