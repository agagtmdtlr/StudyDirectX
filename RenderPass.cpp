#include "stdafx.h"
#include "RenderPass.h"
#include <winerror.h>
#include <d3dcompiler.h>
#include <d3dcompiler.inl>
#include "RenderCommon.h"
#include <d3d11shader.h>

//D3DReflect unrsolved external symbol
#pragma comment(lib, "dxguid.lib") 


RenderPass::RenderPass(std::wstring shaderName , RenderPassState state)
{
	Initialize(shaderName, state );
}



void RenderPass::Initialize(std::wstring shaderName, RenderPassState state)
{
	this->state = state;

	std::wstring shaderPath = L"Shader/" + shaderName + L".hlsl";


	ID3DBlob* errorBlob = nullptr;

	ID3D11Device* device = RenderContext::GetDevice();

	/* Compile HLSL */
	if (state.CheckMask(RenderStage::VS) == true)
	{

		if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, 0, "VSmain", "vs_5_0", 0, 0, GetBlobAddressOf(RenderStage::VS), &errorBlob)))
		{
			if (errorBlob)
			{
				std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}

		ID3DBlob* blob = GetBlob(RenderStage::VS);

		const auto& ied = RenderDescriptor::GetVertexElementDesc();
		device->CreateInputLayout(ied.data(), (UINT)ied.size(), blob->GetBufferPointer(), blob->GetBufferSize(), layout.GetAddressOf());
		
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vertexShader);

	}

	if (state.CheckMask(RenderStage::PS) == true)
	{

		if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, 0, "PSmain", "ps_5_0", 0, 0, GetBlobAddressOf(RenderStage::PS), &errorBlob)))
		{
			if (errorBlob)
			{
				std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}
		ID3DBlob* blob = GetBlob(RenderStage::PS);

		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &pixelShader);

	}

	/* Create Shader Buffer */
	


	for (int st = 0; st < static_cast<int>(RenderStage::NUM); st++)
	{
		RenderStage stage =  static_cast<RenderStage>(st);
		ID3DBlob* blob = GetBlob(stage);
		if (blob != nullptr)
		{
			ID3D11ShaderReflection* reflection = nullptr;
			HRESULT hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflection);

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
					reflection->GetResourceBindingDesc(i, &desc);

					auto& brs = bindResources[std::string(desc.Name)];
					brs.at(st) = desc;
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
	}




	

	
	
}

void RenderPass::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	ID3D11DeviceContext* deviceContext = RenderContext::GetDC();

}

void RenderPass::BeginDraw()
{
	ID3D11Device* device = RenderContext::GetDevice();
	ID3D11DeviceContext* deviceContext = RenderContext::GetDC();

	deviceContext->RSSetViewports(1, &viewport);
	deviceContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
	deviceContext->ClearRenderTargetView(rtv.Get(), &state.clearColor.r);

	deviceContext->IASetInputLayout(layout.Get());

	// set the shader objects
	deviceContext->VSSetShader(vertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(pixelShader.Get(), 0, 0);

	/*
	bindreosurce mep
	*/

	if (state.CheckMask(RenderStage::PS))
	{
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0,1,vertexBuffer.GetAddressOf(), &stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
}

void RenderPass::EndDraw()
{

}

ID3DBlob* RenderPass::GetBlob(RenderStage stage)
{
	return blobs[static_cast<size_t>(stage)].Get();
}

ID3DBlob** RenderPass::GetBlobAddressOf(RenderStage stage)
{
	return blobs[static_cast<size_t>(stage)].GetAddressOf();

}
