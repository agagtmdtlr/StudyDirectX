#include "stdafx.h"
#include "Shader.h"
#include <winerror.h>
#include <d3dcompiler.h>
#include <d3dcompiler.inl>
#include "RenderCommon.h"
#include <d3d11shader.h>

//D3DReflect unrsolved external symbol
#pragma comment(lib, "dxguid.lib") 




Shader::Shader(std::wstring shaderName , RenderPassState state)
{
	Initialize(shaderName, state );
}

void Shader::Initialize(std::wstring shaderName, RenderPassState state)
{
	this->state = state;

	std::wstring shaderPath = L"Shader/" + shaderName + L".hlsl";


	ID3DBlob* errorBlob = nullptr;

	ID3D11Device* device = D3D::GetDevice();

	D3D_SHADER_MACRO macro[] = { "DEBUG" , "0"};


	/* Compile HLSL */
	if (state.CheckMask(RenderStage::VS) == true)
	{		

		if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSmain", "vs_5_0", 0, 0, GetBlobAddressOf(RenderStage::VS), &errorBlob)))
		{
			if (errorBlob)
			{
				std::cout << "Vertex shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}
		ID3DBlob* blob = GetBlob(RenderStage::VS);
		
		device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &vertexShader);		
	}

	if (state.CheckMask(RenderStage::PS) == true)
	{

		if (FAILED(D3DCompileFromFile(shaderPath.c_str(), 0, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PSmain", "ps_5_0", 0, 0, GetBlobAddressOf(RenderStage::PS), &errorBlob)))
		{
			if (errorBlob)
			{
				std::cout << "Pixel shader compile error\n" << (char*)errorBlob->GetBufferPointer() << std::endl;
			}
		}
		ID3DBlob* blob = GetBlob(RenderStage::PS);

		device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), NULL, &pixelShader);

	}

	/* Reflect Shader */
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

				// only vs
				if (RenderStage(st) == RenderStage::VS)
				{
					InitializeInputLayout(shaderDesc, reflection, blob);
				}

				for (UINT i = 0; i < shaderDesc.ConstantBuffers; ++i)
				{
					ID3D11ShaderReflectionConstantBuffer* constantBuffer = reflection->GetConstantBufferByIndex(i);
					D3D11_SHADER_BUFFER_DESC desc;
					D3D11_SHADER_INPUT_BIND_DESC inputDesc;
					if (SUCCEEDED(constantBuffer->GetDesc(&desc)))
					{
						std::string name =std::string(desc.Name);
						if (constantBuffers.find(name) == constantBuffers.end())
						{	
							constantBuffers.insert( make_pair( name , ConstantBuffer(desc.Size) ) );
						}
					}
				} // for ConstantBuffers

				for (UINT i = 0; i < shaderDesc.BoundResources; ++i)
				{
					D3D11_SHADER_INPUT_BIND_DESC desc;
					reflection->GetResourceBindingDesc(i, &desc);

					std::string name = std::string(desc.Name);
					if (bindDescMap.find(name) == bindDescMap.end())
					{
						bindDescMap[name].desc = desc;
						bindDescMap[name].mask.reset();
					}
					
					bindDescMap[name].mask[st] = true;
				} // for BoundResources

				for (UINT i = 0; i < shaderDesc.OutputParameters; ++i)
				{
					D3D11_SIGNATURE_PARAMETER_DESC desc;
					reflection->GetOutputParameterDesc(i, &desc);

				} // for OutputParameters
			} // HRESULT
		} // blob
	} // for renderStage
}

void Shader::InitializeInputLayout(D3D11_SHADER_DESC shaderDesc, ID3D11ShaderReflection* reflection , ID3DBlob* blob)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT i = 0; i < shaderDesc.InputParameters; ++i)
	{
		D3D11_SIGNATURE_PARAMETER_DESC desc;
		reflection->GetInputParameterDesc(i, &desc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = desc.SemanticName;
		elementDesc.SemanticIndex = desc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InstanceDataStepRate = 0;

		if (desc.Mask == 1)
		{
			if(desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if(desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (desc.Mask <= 3)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (desc.Mask <= 7)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (desc.Mask <= 15)
		{
			if (desc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		inputLayoutDesc.push_back(elementDesc);
	}

	auto dc = D3D::GetDevice();
	HRESULT result = dc->CreateInputLayout(inputLayoutDesc.data(), inputLayoutDesc.size(), blob->GetBufferPointer(), blob->GetBufferSize(), inputlayout.GetAddressOf());
	if (FAILED(result))
	{
		std::cout << "failed create input layout" << std::endl;
	}
}

void Shader::DrawIndexed(UINT indexCount, UINT startIndexLocation, INT baseVertexLocation)
{
	ID3D11DeviceContext* dc = D3D::GetDC();
	dc->DrawIndexed(indexCount,startIndexLocation,baseVertexLocation);

}

void Shader::BeginDraw(ID3D11RenderTargetView* rtv)
{
	ID3D11Device* device = D3D::GetDevice();
	ID3D11DeviceContext* dc = D3D::GetDC();

	dc->RSSetViewports(1, &viewport);
	dc->OMSetRenderTargets(1, &rtv, nullptr);
	dc->ClearRenderTargetView(rtv, &state.clearColor.x);


	// set the shader objects
	if (state.CheckMask(RenderStage::VS))
	{
		dc->VSSetShader(vertexShader.Get(), 0, 0);
		dc->IASetInputLayout(inputlayout.Get());
	}

	if (state.CheckMask(RenderStage::PS))
	{
		dc->PSSetShader(pixelShader.Get(), 0, 0);
	
	}

	/*
	bindreosurce mep
	*/
	for (auto& it : constantBuffers)
	{
		BindConstantBufferToStage(it.first, it.second);
	}
}

void Shader::EndDraw()
{
	ID3D11DeviceContext* dc = D3D::GetDC();
	dc->VSSetShader(nullptr, 0, 0);
	dc->PSSetShader(nullptr, 0, 0);
	dc->IASetInputLayout(nullptr);
}

ID3DBlob* Shader::GetBlob(RenderStage stage)
{
	return blobs[static_cast<size_t>(stage)].Get();
}

ID3DBlob** Shader::GetBlobAddressOf(RenderStage stage)
{
	return blobs[static_cast<size_t>(stage)].GetAddressOf();

}

void Shader::BindConstantBufferToStage(std::string name, const ConstantBuffer& cbuffer)
{
	auto dc = D3D::GetDC();
	const BindDesc& bindDesc = bindDescMap[name];

	for (int s = 0 ; s < bindDesc.mask.count(); s++)
	{
		RenderStage stage = RenderStage(s);
		if (state.CheckMask(stage) == true)
		{
			auto& desc = bindDesc.desc;
			switch (stage)
			{
			case RenderStage::VS:
				dc->VSSetConstantBuffers(desc.BindPoint, desc.BindCount, cbuffer.GetAddressOf() );
				break;
			case RenderStage::PS:
				dc->PSSetConstantBuffers(desc.BindPoint, desc.BindCount, cbuffer.GetAddressOf());
				break;
			case RenderStage::CS:
				dc->CSSetConstantBuffers(desc.BindPoint, desc.BindCount, cbuffer.GetAddressOf());
				break;
			default:
				break;
			}
		}
	}
}

void Shader::BindPrimitiveBuffer(PrimitiveBuffer* buffer)
{
	auto dc = D3D::GetDC();
	UINT stride = buffer->GetVertexStride();
	UINT offset = 0;

	dc->IASetVertexBuffers(0, 1, buffer->GetVertexBufferAddressOf(), &stride, &offset);
	dc->IASetIndexBuffer(buffer->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);

	dc->IASetPrimitiveTopology(buffer->GetTopology());
}

bool Shader::UpdateConstantData(std::string name, Update_ConstantBuffe_Desc desc)
{
	if (constantBuffers.find(name) == constantBuffers.end())
	{
		std::cout << "cant find update ConstantBuffer by name (" << name << ")" << endl;
		return false;
	}
	ConstantBuffer& cbuffer = constantBuffers[name];
	cbuffer.UpdateData(desc);

	return true;
}

bool Shader::BindSRV(std::string name, ID3D11ShaderResourceView* srv)
{
	auto dc = D3D::GetDC();

	const BindDesc& bindDesc = bindDescMap[name];
	for (int s = 0; s < bindDesc.mask.count(); s++)
	{
		RenderStage stage = RenderStage(s);
		if (state.CheckMask(stage) == true)
		{
			auto& desc = bindDesc.desc;
			switch (stage)
			{
			case RenderStage::VS:
				dc->VSSetShaderResources(desc.BindPoint, desc.BindCount, &srv);
				break;
			case RenderStage::PS:
				dc->PSSetShaderResources(desc.BindPoint, desc.BindCount, &srv);
				break;
			case RenderStage::GS:
				dc->GSSetShaderResources(desc.BindPoint, desc.BindCount, &srv);
				break;
			case RenderStage::CS:
				dc->CSSetShaderResources(desc.BindPoint, desc.BindCount, &srv);
				break;
			default:
				break;
			}
		}
	}
	return true;
}

void Shader::BindRenderTargets(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
	auto dc = D3D::GetDC();
	dc->OMSetRenderTargets(1,&rtv, dsv);
}

void Shader::BindDepthStencilState(ID3D11DepthStencilState* state)
{
	auto dc = D3D::GetDC();
	dc->OMSetDepthStencilState(state, 0);
}

bool Shader::BindSampler(std::string name, ID3D11SamplerState* sampler)
{
	auto dc = D3D::GetDC();

	const BindDesc& bindDesc = bindDescMap[name];
	for (int s = 0; s < bindDesc.mask.count(); s++)
	{
		RenderStage stage = RenderStage(s);
		if (state.CheckMask(stage) == true)
		{
			auto& desc = bindDesc.desc;
			switch (stage)
			{
			case RenderStage::VS:
				dc->VSSetSamplers(desc.BindPoint, desc.BindCount, &sampler);
				break;
			case RenderStage::PS:
				dc->PSSetSamplers(desc.BindPoint, desc.BindCount, &sampler);
				break;
			case RenderStage::GS:
				dc->GSSetSamplers(desc.BindPoint, desc.BindCount, &sampler);
				break;
			case RenderStage::CS:
				dc->CSSetSamplers(desc.BindPoint, desc.BindCount, &sampler);
				break;
			default:
				break;
			}
		}		
	}
	return true;
}
