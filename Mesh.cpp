#include "stdafx.h"
#include "Mesh.h"
#include <memory>
#include <iostream>
#include "RenderContext.h"

Mesh::Mesh(void* vertices, UINT vertexStride, UINT vertexCount, void* indices, UINT indexCount)
{
	// Create an vertex buffer
	{
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc,sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;	 // read only GPU
		bufferDesc.ByteWidth = vertexStride * vertexCount;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
		bufferDesc.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData = {0,};
		vertexBufferData.pSysMem = vertices;
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		const HRESULT hr = D3D::GetDevice()->CreateBuffer(&bufferDesc,&vertexBufferData,vertextBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "CreateBuffer() failed : vertexBuffer" << std::hex << hr << std::endl;
		}
	}

	// Create an index buffer
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indexCount);
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // usage에서 cpu write를 허용하지 않는 플래그면 무효하다.
		bufferDesc.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = {};
		indexBufferData.pSysMem = indices;
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		const HRESULT hr = D3D::GetDevice()->CreateBuffer(&bufferDesc, &indexBufferData, indexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "CrateBuffer() failed : indexBuffer" << std::hex << hr << std::endl;
		}
	}
}
