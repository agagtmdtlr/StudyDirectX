#include "stdafx.h"
#include "RenderPass.h"
#include <d3dcompiler.h>
#include <winerror.h>
#include <d3dcompiler.inl>

RenderPass::RenderPass(std::wstring shaderName)
{
	Initialize(shaderName);
}

void RenderPass::Initialize(std::wstring shaderName)
{
	ID3DBlob* vertexBlob = nullptr;
	ID3DBlob* pixelBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::wstring shaderPath = L"Shader/" + shaderName;


	if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, 0, "VSmain", "vs_5_0", 0, 0, &vertexBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, 0, "PSmain", "ps_5_0", 0, 0, &pixelBlob, &errorBlob)))
	{
		if (errorBlob)
		{
			std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
		}
	}

	ID3D11Device* device = RenderContext::GetRenderContext()->GetDevice();

	device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);

	ID3D11ShaderReflection* reflection = nullptr;
	HRESULT hr = D3DReflect(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);

	// reflection shader resource
	if (SUCCEEDED(hr))
	{
		D3D11_SHADER_DESC shaderDesc;
		reflection->GetDesc(&shaderDesc);

		for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
		{
			ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
			D3D11_SHADER_BUFFER_DESC desc;
			constantBuffer->GetDesc(&desc);
		}

		for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
		{
			D3D11_SHADER_INPUT_BIND_DESC desc;
			reflection->GetResourceBindingDesc(i,&desc);
		}

		for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC desc;
			reflection->GetInputParameterDesc(i, &desc);
		}

		for (UINT i = 0; i < shaderDesc.OutputParameters; ++i)
		{
			D3D11_SIGNATURE_PARAMETER_DESC desc;
			reflection->GetOutputParameterDesc(i, &desc);
			
		}

	}

}
