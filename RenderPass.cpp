#include "stdafx.h"
#include "RenderPass.h"
#include <winerror.h>
#include <d3dcompiler.h>
#include <d3dcompiler.inl>
#include "RenderCommon.h"
#include <d3d11shader.h>

//D3DReflect unrsolved external symbol
#pragma comment(lib, "dxguid.lib") 


bool RenderPass::BindConstantBuffer(std::wstring resourceName, ID3D11Resource* resource)
{
	

	return true;
}

RenderPass::RenderPass(std::wstring shaderName , RenderPassState state)
{
	Initialize(shaderName, state );
}

void RenderPass::Initialize(std::wstring shaderName, RenderPassState state)
{
	this->state = state;

	std::wstring shaderPath = L"Shader/" + shaderName + L".hlsl";


	ID3DBlob* errorBlob = nullptr;

	ID3D11Device* device = D3D::GetDevice();

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
					brs[st] = desc;
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
	ID3D11DeviceContext* deviceContext = D3D::GetDC();

}

void RenderPass::BeginDraw()
{
	ID3D11Device* device = D3D::GetDevice();
	ID3D11DeviceContext* dc = D3D::GetDC();

	dc->RSSetViewports(1, &viewport);
	dc->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
	dc->ClearRenderTargetView(rtv.Get(), &state.clearColor.r);

	dc->IASetInputLayout(layout.Get());

	// set the shader objects
	dc->VSSetShader(vertexShader.Get(), 0, 0);
	dc->PSSetShader(pixelShader.Get(), 0, 0);

	/*
	bindreosurce mep
	*/

	if (state.CheckMask(RenderStage::PS))
	{
		if( primitiveBuffer != nullptr)
			primitiveBuffer->BeginDraw();
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
