#include "stdafx.h"
#include "Renderer.h"
#include "RenderCommon.h"
#include "FileManager.h"
#include "ServiceLoactor.h"
#include "imgui.h"
Renderer::Renderer()
{
	ServiceLoactor::RegisterService<Renderer>(this);
}

void Renderer::Update()
{
	meshesForDraw.clear();
}

void Renderer::UpdateCamera(const Camera& camera)
{
	this->camera = camera;
}

void Renderer::reserveDrawMesh(shared_ptr<Mesh> mesh)
{
	meshesForDraw.push_back(mesh);
}

void Renderer::Render()
{	
	//renderPass->BeginDraw(primitive.get(), D3D::GetRTV());
	//renderPass->DrawIndexed(primitive->GetIndexCount(), 0, 0);
	auto dc = D3D::GetDC();

	Resize(D3D::GetDisplayWidth(),D3D::GetDisplayHeight());	


	Color clearColor = Color(0,0,0,1);
	dc->ClearRenderTargetView(D3D::GetDisplayRTV(), &clearColor.x);
	dc->ClearDepthStencilView(D3D::GetDisplayDSV(),D3D11_CLEAR_DEPTH,1.0f,0);
	renderPass->BeginDraw(D3D::GetDisplayRTV(), D3D::GetDisplayDSV());

	ImGui::Begin("Renderer");

	UINT s = 0;
	static bool isfirst = true;
	static bool* barray = nullptr;

	if (isfirst)
	{
		for (auto mesh : meshesForDraw)
		{
			s+=mesh->GetBufferCount();
		}
		isfirst = false;
		barray = new bool[s];
	}

	UINT btnI = 0;
	for (auto mesh : meshesForDraw)
	{
		

		MatrixBufferType cbuffer;
		// transpose for align to hlsl matrix major

		cbuffer.world = mesh->GetTransformRef()->GetWorldMatrix().Transpose();
		cbuffer.view = camera.GetView().Transpose();
		cbuffer.projection = camera.GetProjection().Transpose();
		{
			Update_ConstantBuffe_Desc desc;
			desc.data = &cbuffer;
			desc.dataSize = sizeof(MatrixBufferType);
			renderPass->UpdateConstantData("MatrixBuffer", desc);
		}
		

		for (UINT i = 0; i < mesh->GetBufferCount(); i++)
		{
			string btnName = "Mesh_" + to_string(btnI);
			btnI++;

			ImGui::Checkbox(btnName.c_str(), &(barray[btnI]));
			if (barray[btnI] == false)
			{
				continue;
			}

			auto p = mesh->GetBuffer(i);
			renderPass->BindPrimitiveBuffer(p);
			renderPass->DrawIndexed(p->GetIndexCount(), 0, 0);
		}
	}

	ImGui::End();


	renderPass->EndDraw();
}

void Renderer::BeginUI()
{
	auto dc = D3D::GetDC();
	
	dc->RSSetViewports(1, D3D::GetViewport());
	Color clearColor(0, 0, 0, 1);
	auto rtv = D3D::GetRTV();
	dc->ClearRenderTargetView(rtv, &clearColor.x);
	dc->OMSetRenderTargets(1,&rtv,nullptr);
}

void Renderer::Resize(int width, int height)
{
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
	renderPass->viewport = viewport;

}

void Renderer::Initialize(int width, int height)
{
	auto device = D3D::GetDevice();
	auto deviceContext = D3D::GetDC();
	RenderPassState state;
	state.SetFlag(RenderStage::VS,true);
	state.SetFlag(RenderStage::PS,true);
	state.clearColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

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
	renderPass = std::make_unique<Shader>(L"Phong",state);
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
	camera.fov = 45.f;
	camera.position = Vector3(0,0,-5);


	camera.nearPlane = 0.01f;
	camera.farPlane = 1000000.0f;

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
			{ {-1.0f, -1.0f, 0.0f},{0,0,-1},{0,0} },
			{ {0.0f, 1.0f, 0.0f},{0,0,-1},{0,0}  },
			{ {1.0f, -1.0f, 0.0f},{0,0,-1},{0,0} }
		};

		std::vector<UINT> indices =
		{
			0,1,2
		};

		primitive = make_shared<PrimitiveBuffer>();
		primitive->Initialize(vertices,indices);
	}

	texture = TextureManager::RequestTexture(L"abstract.jpg");
}


