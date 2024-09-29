#include "stdafx.h"
#include "PrimitiveBuffer.h"

PrimitiveBuffer::PrimitiveBuffer(vector<Vertex>& vertices, vector<UINT>& indices)
	: vertices(vertices), indices(indices)
{
	auto device = D3D::GetDevice();
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = sizeof(Vertex) * vertices.size();
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
		if(FAILED(result))
			cout << "failed create vertex buffer" << endl;
	}
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = sizeof(UINT32) * indices.size();
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = indices.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&desc, &data, indexBuffer.GetAddressOf());
		if (FAILED(result))
			cout << "failed create index buffer" << endl;
	}

}

UINT PrimitiveBuffer::GetVertexStride()
{
	return sizeof(Vertex);
}
