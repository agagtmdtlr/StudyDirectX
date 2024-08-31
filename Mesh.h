#pragma once
#include <d3d11.h>
#include <wrl/client.h>


class Mesh
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertextBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Mesh(void* vertices, UINT vertexStride, UINT vertexCount, void* indices, UINT indexCount );

	UINT vertexCount;
	UINT indexCount;
	UINT stride;
	UINT instanceCount;

};

