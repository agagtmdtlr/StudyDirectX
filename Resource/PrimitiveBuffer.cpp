#include "stdafx.h"
#include "PrimitiveBuffer.h"

unordered_map<string, shared_ptr<PrimitiveBufferContainer>> PrimitiveBufferManager::bufferMap;

PrimitiveBuffer::PrimitiveBuffer()
{
}

void PrimitiveBuffer::Initialize(vector<Vertex>& vertices, vector<UINT>& indices)
{
	auto device = D3D::GetDevice();
	indexCount = (UINT)indices.size();
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		desc.ByteWidth = (UINT)(sizeof(Vertex) * vertices.size());
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA data;
		ZeroMemory(&data, sizeof(data));
		data.pSysMem = vertices.data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT result = device->CreateBuffer(&desc, &data, vertexBuffer.GetAddressOf());
		if (FAILED(result))
			cout << "failed create vertex buffer" << endl;
	}
	{
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = (UINT)(sizeof(UINT32) * indices.size());
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

bool PrimitiveBufferManager::IsExisted(std::string name)
{
	return bufferMap.find(name) != bufferMap.end();
}

void PrimitiveBufferManager::RegistBuffer(std::string name, vector<Vertex>& vertices, vector<UINT>& indices)
{
	if (IsExisted(name) == false)
	{
		std::shared_ptr<PrimitiveBufferContainer> buffer = make_shared<PrimitiveBufferContainer>();
		bufferMap.insert(
			make_pair(name, buffer)
		);

		bufferMap[name]->name = name;
	}
	auto& container = bufferMap[name];
	container->buffers.emplace_back(make_unique<PrimitiveBuffer>());
	container->buffers.back()->Initialize(vertices, indices);
}

std::shared_ptr<PrimitiveBufferContainer> PrimitiveBufferManager::RequestBuffer(std::string name)
{
	return bufferMap[name];
}

