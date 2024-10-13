#pragma once

struct Update_ConstantBuffe_Desc
{
	void* data;
	UINT dataSize;
};

class ConstantBuffer
{
public:
	ConstantBuffer();
	void Initialize(UINT bufferSize);
	bool UpdateData(const Update_ConstantBuffe_Desc& desc);

	ID3D11Buffer*const* GetAddressOf() const { return buffer.GetAddressOf(); }

	size_t GetSize() { return bufferSize; }
private:
	ComPtr<ID3D11Buffer> buffer;
	size_t bufferSize;
};













