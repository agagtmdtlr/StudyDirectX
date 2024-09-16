#include "stdafx.h"
#include "RenderDemo.h"

RenderDemo::RenderDemo(HWND window, int width, int height)
{
}

void RenderDemo::Update()
{
}

void RenderDemo::Render()
{
}

void RenderDemo::Initialize(HWND window, int width, int height)
{
	RenderContext* renderContext = RenderContext::GetRenderContext();
	renderContext->Initialize(window, width, height);
	auto device = renderContext->GetDevice();
	auto deviceContext = renderContext->GetDC();
	RenderPassState state;

	state.SetFlag(RenderStage::VS,true);
	state.SetFlag(RenderStage::PS,true);
	state.clearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	D3D11_VIEWPORT viewport;
	// Set the viewport
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = float(width);
	viewport.Height = float(height);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f; // Note: important for depth buffering

	renderPass = std::make_unique<RenderPass>("PhongShader.hlsl",state);
	renderPass->viewport = viewport;

	// Create texture and rendertarget
	D3D11_SAMPLER_DESC sampDesc;
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

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
	textureDesc.MipLevels = textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DYNAMIC;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.MiscFlags = 0;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	//textureDesc.Width = width;
	//textureDesc.Height = height;
	textureDesc.Width = width;
	textureDesc.Height = height;

	device->CreateTexture2D(&textureDesc, nullptr, canvasTexture.GetAddressOf());


	if (canvasTexture)
	{
		device->CreateShaderResourceView(canvasTexture.Get(), nullptr, &canvasTextureView);
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
		renderTargetViewDesc.Format = textureDesc.Format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetViewDesc.Texture2D.MipSlice = 0;

		device->CreateRenderTargetView(canvasTexture.Get(), &renderTargetViewDesc, canvasRenderTargetView.GetAddressOf());

	}
	else
	{
		std::cout << "CreateRenderTargetView() error : canvasRenderTargetView" << std::endl;
	}

	// Create a vertex Buffer 
	{
		const std::vector<Vertex> vertices =
		{
			{ {-1.0f, -1.0f , 0.0f , 1.0f}, {0.f,1.0f},},
			{ {1.0f, -1.0f, 0.0f, 1.0f}, {1.0f,1.0f},},
			{ {1.0f, 1.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
			{ {-1.0f,1.0f,0.0f,1.0f}, {0.f,0.f}},
		};

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;		// wirte access by CPU and GPU
		bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size()); // size is the vertex struct * 4
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //  allow CPU to write in buffer
		bufferDesc.StructureByteStride = sizeof(Vertex);

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0, };
		vertexBufferData.pSysMem = vertices.data();
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		const HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexBufferData, &vertexBuffer);
		if (FAILED(hr))
		{
			std::cout << "CreateBuffer() failed : vertexBuffer" << std::hex << hr << std::endl;
		}
	}

	// Create an index buffer
	{
		const std::vector<uint16_t> indices =
		{
			3,1,0,
			2,1,3,
		};

		indexCount = UINT(indices.size());

		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = UINT(sizeof(uint16_t) * indices.size());
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;       // use as a vertex buffer
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // usage에서 cpu write를 허용하지 않는 플래그면 무효하다.
		bufferDesc.StructureByteStride = sizeof(uint16_t);

		D3D11_SUBRESOURCE_DATA indexBufferData = {};
		indexBufferData.pSysMem = indices.data();
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;
		const HRESULT hr = device->CreateBuffer(&bufferDesc, &indexBufferData, &indexBuffer);
		if (FAILED(hr))
		{
			std::cout << "CrateBuffer() failed : indexBuffer" << std::hex << hr << std::endl;
		}
	}



}


