#pragma once

enum class TextureExtension // extension 의존성 불필요.
{
	png = 0,
	jpg = 1,
	dds = 2,
	hdr = 3
};

struct ITextureDescriptor
{
};

struct TextureDescriptor
{
	int width;
	int height;
	TextureExtension extension;
};