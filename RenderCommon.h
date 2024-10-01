#pragma once
#include <algorithm>
#include <math.h>
#include <glm/glm.hpp>
#include <vector>
#include <d3d11.h>
#include <d3dcompiler.h>


struct Vertex
{
	Vector4 position;
	Vector3 normal;
};

class RenderDescriptor
{
public:
	static const std::vector< D3D11_INPUT_ELEMENT_DESC>& GetVertexElementDesc()
	{
		// Create the input layout object
		static std::vector< D3D11_INPUT_ELEMENT_DESC> ied(
			{
				{ "POSITION",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 }
			}
		);

		return ied;
	}
};

struct MatrixBufferType
{
	Matrix world;
	Matrix view;
	Matrix projection;
};
