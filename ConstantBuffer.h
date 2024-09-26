#pragma once

class ConstantBuffer
{
public:
	ConstantBuffer(UINT bufferSize);

	bool UpdateData(void* data, UINT dataSize);

	ID3D11Buffer*const* GetAddressOf() const { return buffer.GetAddressOf(); }

	size_t GetSize() { return bufferSize; }
private:
	ComPtr<ID3D11Buffer> buffer;
	size_t bufferSize;
};











