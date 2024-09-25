#pragma once

template <typename T>
class ConstantBuffer
{
public:
	ConstantBuffer();

	bool UpdateData(T* data);
private:
	ComPtr<ID3D11Buffer> buffer;
};

template<typename T>
inline ConstantBuffer<T>::ConstantBuffer()
{
	auto device = D3D::GetDevice();

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof(T);
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem = nullptr;
	subData.SysMemPitch = 0;
	subData.SysMemSlicePitch = 0;

	HRESULT result = device->CreateBuffer(&desc,nullptr, buffer.GetAddressOf());
	if(FAILED(result))
		cout << "failed create constant buffer" << endl;
	
}

template<typename T>
inline bool ConstantBuffer<T>::UpdateData(T* data)
{
	auto dc = D3D::GetDC();

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	// Lock the constant buffer so it can be written to
	HRESULT result = dc->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
		return false;

	gpuData = (T*)mappedResource.pData;
	*gpuData = data;

	// Unlock the Constant Buffer
	dc->Unmap(buffer, 0);

	return true;
}

