#include "Shader.h"
#include <d3dcompiler.h>
#include <iostream>
#include "RenderContext.h"

void Shader::InitShader()
{
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	if (FAILED(D3DCompileFromFile(L"Shader/VS.hlsl", 0, 0, "main", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	if (FAILED(D3DCompileFromFile(L"Shader/PS.hlsl", 0, 0, "main", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	ID3D11Device* device = RenderContext::GetRenderContext()->device.Get();
	ID3D11DeviceContext* deviceContex = RenderContext::GetRenderContext()->deviceContext.Get();

	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);

	
}
