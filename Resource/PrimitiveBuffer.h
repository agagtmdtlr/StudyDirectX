#pragma once

class PrimitiveBuffer
{	
public:
	PrimitiveBuffer();
	void Initialize(vector<Vertex>& vertices, vector<UINT>& indices);

	UINT GetVertexStride();
	UINT GetIndexCount() { return indexCount; }
	ID3D11Buffer** GetVertexBufferAddressOf() { return vertexBuffer.GetAddressOf(); }
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer.Get(); }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return topology; }

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	UINT indexCount = 0;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

class PrimitiveBufferContainer
{
public:
	PrimitiveBufferContainer() {};

	vector<std::unique_ptr<PrimitiveBuffer>> buffers;
	string name;
};

class PrimitiveBufferManager
{
public:
	static bool IsExisted(std::string name);
	static void RegistBuffer(std::string name, vector<Vertex>& vertices, vector<UINT>& indices);
	static std::shared_ptr<PrimitiveBufferContainer> RequestBuffer(std::string name);
	
private:
	static unordered_map<string, shared_ptr<PrimitiveBufferContainer>> bufferMap;

};