#include "stdafx.h"
#include "ConstantBuffer.h"


ConstantBuffer::ConstantBuffer()
{
}



void ConstantBuffer::Initialize(UINT bufferSize)
{
	this->bufferSize = bufferSize;

	auto device = D3D::GetDevice();

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = bufferSize;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = nullptr;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
	if (FAILED(result))
		cout << "failed create constant buffer" << endl;

}

bool ConstantBuffer::UpdateData(const Update_ConstantBuffe_Desc& desc)
{
	auto dc = D3D::GetDC();
	void* data = (MatrixBufferType*)desc.data;
	UINT dataSize = desc.dataSize;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Lock the constant buffer so it can be written to
	HRESULT result = dc->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	void* gpuData = mappedResource.pData;
	memcpy(gpuData, data, dataSize);

	// Unlock the Constant Buffer
	dc->Unmap(buffer.Get(), 0);

	return true;
}