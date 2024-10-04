#include "stdafx.h"
#include "Renderer.h"
#include "RenderCommon.h"
#include "FileManager.h"

Renderer::Renderer()
{
}

void Renderer::Update()
{
}

void Renderer::Render()
{	
	//renderPass->BeginDraw(primitive.get(), D3D::GetRTV());
	//renderPass->DrawIndexed(primitive->GetIndexCount(), 0, 0);

	renderPass->BeginDraw(sphere->GetBuffer(), D3D::GetRTV());
	renderPass->DrawIndexed(sphere->GetIndexCount(), 0, 0);
	renderPass->EndDraw();
}

void Renderer::Initialize(int width, int height)
{
	auto device = D3D::GetDevice();
	auto deviceContext = D3D::GetDC();
	RenderPassState state;
	state.SetFlag(RenderStage::VS,true);
	state.SetFlag(RenderStage::PS,true);
	state.clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	D3D11_VIEWPORT viewport;
	{
		// Set the viewport
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(width);
		viewport.Height = float(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f; // Note: important for depth buffering
	}
	
	// render pass
	renderPass = std::make_unique<RenderPass>(L"Phong",state);
	renderPass->viewport = viewport;

	// Create texture and rendertarget
	D3D11_SAMPLER_DESC sampDesc;
	{
		ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // 
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

		//Create the Sample State
		device->CreateSamplerState(&sampDesc, colorSampler.GetAddressOf());
	}
	
	camera.aspectRatio = D3D::GetWndAspectRatio();
	camera.fov = XMConvertToRadians(45.f);
	camera.position = Vector3(0,0,0);
	camera.nearPlane = 0.01f;
	camera.farPlane = 1000.0f;

	MatrixBufferType cbuffer;
	// transpose for align to hlsl matrix major
	cbuffer.world = Matrix::Identity;
	cbuffer.world = cbuffer.world.Transpose();
	cbuffer.view = camera.GetView().Transpose();
	cbuffer.projection = camera.GetProjection().Transpose();

	{
		Update_ConstantBuffe_Desc desc;
		desc.data = &cbuffer;
		desc.dataSize = sizeof(cbuffer);
		renderPass->UpdateConstantData("MatrixBuffer", desc);
	}

	// Create a vertex Buffer 
	{
		float s = 1.0f;
		float z = 0.5f;
		std::vector<Vertex> vertices =
		{
			{ {-1.0f, -1.0f, 0.0f, 1.f},{0,0,-1} },
			{ {0.0f, 1.0f, 0.0f, 1.f},{0,0,-1} },
			{ {1.0f, -1.0f, 0.0f, 1.f},{0,0,-1}}
		};

		std::vector<UINT> indices =
		{
			0,1,2
		};

		primitive = make_shared<PrimitiveBuffer>(vertices, indices);
	}

	texture = TextureManager::RequestTexture(L"abstract.jpg");

	
	sphere = make_unique<Sphere>();
	sphere->Initialize("sphere",Transform());
	


}


