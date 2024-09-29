#pragma once

class PrimitiveBuffer
{	
public:
	PrimitiveBuffer(vector<Vertex>& vertices, vector<UINT>& indices);

	UINT GetVertexStride();
	UINT GetIndexCount() { return indices.size(); }
	ID3D11Buffer** GetVertexBufferAddressOf() { return vertexBuffer.GetAddressOf(); }
	ID3D11Buffer* GetIndexBuffer() { return indexBuffer.Get(); }
	D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return topology; }

private:
	vector<Vertex> vertices;
	vector<UINT> indices;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

