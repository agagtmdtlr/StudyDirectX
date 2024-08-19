#pragma once

class FileManager
{
public:
	static unsigned char* LoadImage(const char* filename, int* width, int* height, int* channels);
	static void WriteImage(const char* filename, int width, int height, int channels, const void* data);
};

