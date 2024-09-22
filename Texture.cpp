#include "stdafx.h"
#include "Texture.h"

Texture::Texture(TexMetadata metaData, ID3D11ShaderResourceView* srv)
	:metaData(metaData), srv(srv)
{
}
