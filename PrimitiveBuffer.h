#pragma once
class PrimitiveBuffer
{	
public:
	PrimitiveBuffer(vector<Vertex>& vertices, vector<UINT32>& indices);
	void BeginDraw();

private:
	vector<Vertex> vertices;
	vector<UINT32> indices;

	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;

	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};

