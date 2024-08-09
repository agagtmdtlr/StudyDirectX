#pragma once

#include <windows.h>
#include <memory>
#include <iostream>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <chrono>
#include <algorithm>
#include <glm/glm.hpp>
#include "RenderCommon.h"
#include "Image.h"
#include "Circle.h"


#define SafeRelease(com) { if(com) { com->Release(); com = NULL;}}



struct Vertex
{
	glm::vec4 pos;
	glm::vec2 uv;
};

class Example
{

public:
	// https://github.com/Microsoft/DirectXTK/wiki/Implementation#naming-conventions
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	IDXGISwapChain* swapChain;
	D3D11_VIEWPORT viewport;
	ID3D11RenderTargetView* renderTargetView;
	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11InputLayout* layout;

	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
	ID3D11Texture2D* canvasTexture = nullptr;
	ID3D11ShaderResourceView* canvasTextureView = nullptr;
	ID3D11RenderTargetView* canvasRenderTargetView = nullptr;
	ID3D11SamplerState* colorSampler;
	UINT indexCount;

	int width, height;
	float backgroundColor[4] = { 0.8f,0.8f,0.8f,1.0f };
	float canvasColor[4] = { 0 };

	std::unique_ptr<slab::Circle> circle;

	Image image;
public:
	Example(HWND window, int width, int height)
	{
		//image.ReadFromFile("../Resources/image_1.jpg");
		
		// 시간 측정
		const auto start_time = std::chrono::high_resolution_clock::now();
		const auto elapsed_time = std::chrono::high_resolution_clock::now() - start_time;

		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count() / 1000.0 << " sec" << std::endl;
		
		//image.WritePNG("../Resources/result.png");

		circle = std::make_unique<slab::Circle>(slab::Circle({width/2.f,height/2.f}, 100.f, {1.f,0.f,0.f,1.f}));

		Initialize(window, width, height);
	}

	// https://learn.microsoft.com/en-us/windows/win32/direct3d11/how-to--compile-a-shader
	void InitShaders()
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
		
		device->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), NULL , &vertexShader);
		device->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(), NULL, &pixelShader);

		// Create the input layout object
		D3D11_INPUT_ELEMENT_DESC ied[] = 
		{
			{"POSITION",0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		device->CreateInputLayout(ied,2, vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &layout);
		deviceContext->IASetInputLayout(layout);
	}

	void Initialize(HWND window, int width, int height)
	{
		this->width = width;
		this->height = height;

		DXGI_SWAP_CHAIN_DESC swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.BufferDesc.Width = width;
		swapChainDesc.BufferDesc.Height = height;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // use 32-bit color
		swapChainDesc.BufferCount = 2;								  // one back buffer
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // how sawp chain is to bue used
		swapChainDesc.OutputWindow = window;	// the window to be used
		swapChainDesc.SampleDesc.Count = 1;		// houw many multisamples
		swapChainDesc.Windowed = TRUE;			// windowed/full-screen mode
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; // allow full-screen switching
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

		const D3D_FEATURE_LEVEL featureLevelArray[1] = {D3D_FEATURE_LEVEL_11_0};
		if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,
			D3D_DRIVER_TYPE_HARDWARE,
			NULL,
			createDeviceFlags,
			featureLevelArray,
			1,
			D3D11_SDK_VERSION,
			&swapChainDesc,
			&swapChain,
			&device,
			NULL,
			&deviceContext)))
		{
			std::cout << "D3D11CrateDeviceAndSwapChain() error" << std::endl;
		}

		//CreateRenderTarget
		ID3D11Texture2D* pBackBuffer;
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
		if (pBackBuffer)
		{
			device->CreateRenderTargetView(pBackBuffer, NULL, &renderTargetView);
			pBackBuffer->Release();
		}
		else
		{
			std::cout << "CreateRenderTargetView() error" << std::endl;
			exit(-1);
		}

		// Set the viewport
		ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		viewport.Width = float(width);
		viewport.Height = float(height);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f; // Note: important for depth buffering
		deviceContext->RSSetViewports(1, &viewport);

		InitShaders();

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
		device->CreateSamplerState(&sampDesc, &colorSampler);

		D3D11_TEXTURE2D_DESC textureDesc;
		ZeroMemory(&textureDesc, sizeof(D3D11_TEXTURE2D_DESC));
		textureDesc.MipLevels = textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DYNAMIC;
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		textureDesc.MiscFlags =0;
		textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		textureDesc.Width = width;
		textureDesc.Height = height;

		device->CreateTexture2D(&textureDesc, nullptr, &canvasTexture);

		if (canvasTexture)
		{
			device->CreateShaderResourceView(canvasTexture, nullptr, &canvasTextureView);
			D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
			renderTargetViewDesc.Format = textureDesc.Format;
			renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			renderTargetViewDesc.Texture2D.MipSlice = 0;

			device->CreateRenderTargetView(canvasTexture, &renderTargetViewDesc, &canvasRenderTargetView);

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
			ZeroMemory(&bufferDesc,sizeof(D3D11_BUFFER_DESC));
			bufferDesc.Usage =D3D11_USAGE_DYNAMIC;		// wirte access by CPU and GPU
			bufferDesc.ByteWidth = UINT(sizeof(Vertex) * vertices.size()); // size is the vertex struct * 4
			bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER; // use as a vertex buffer
			bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; //  allow CPU to write in buffer
			bufferDesc.StructureByteStride = sizeof(Vertex);

			D3D11_SUBRESOURCE_DATA vertexBufferData = { 0,};
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

			D3D11_SUBRESOURCE_DATA indexBufferData ={};
			indexBufferData.pSysMem = indices.data();
			indexBufferData.SysMemPitch = 0;
			indexBufferData.SysMemSlicePitch =0;
			const HRESULT hr = device->CreateBuffer(&bufferDesc, &indexBufferData, &indexBuffer);
			if (FAILED(hr))
			{
				std::cout << "CrateBuffer() failed : indexBuffer" << std::hex << hr << std::endl;
			}
		}


	}

	void Update()
	{
		std::vector<glm::vec4> pixels(width * height, glm::vec4{ backgroundColor[0],backgroundColor[1] ,backgroundColor[2] ,1 });

		for (int i = 0; i < width; i++ )
		{
			for (int j = 0; j < height; j++)
			{
				if( circle->IsInside(glm::vec2(i,j)) == true )
				{
					pixels[j * width + i] = circle->color;
				}
			}
		}

		// Update texture buffer
		D3D11_MAPPED_SUBRESOURCE ms;
		deviceContext->Map(canvasTexture, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, pixels.data(), pixels.size()* sizeof(glm::vec4));
		deviceContext->Unmap(canvasTexture, NULL);
	}

	void Render()
	{
		float clearColor[4] = { 0.0f , 0.0f , 0.0f , 1.0f};
		deviceContext->RSSetViewports(1,&viewport);
		deviceContext->OMSetRenderTargets(1,&renderTargetView, nullptr);
		deviceContext->ClearRenderTargetView(renderTargetView, clearColor);

		// set the shader objects
		deviceContext->VSSetShader(vertexShader,0,0);
		deviceContext->PSSetShader(pixelShader,0,0);

		// select which vertex buffer to display
		UINT stride = sizeof(Vertex);
		UINT offset = 0;
		deviceContext->IASetVertexBuffers(0,1,&vertexBuffer,&stride, &offset);
		deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT,0);

		// https://github.com/Microsoft/DirectXTK/wiki/Getting-Started
		// https://github.com/microsoft/Xbox-ATG-Samples/tree/main/PCSamples/IntroGraphics/SimpleTexturePC
		deviceContext->PSSetSamplers(0,1 ,&colorSampler);
		deviceContext->PSSetShaderResources(0,1,&canvasTextureView);
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		deviceContext->DrawIndexed(indexCount ,0,0);

	}

	void Clean()
	{
		
		SafeRelease(layout);
		SafeRelease(vertexShader);
		SafeRelease(pixelShader);
		SafeRelease(vertexBuffer);
		SafeRelease(indexBuffer);
		SafeRelease(canvasRenderTargetView);
		SafeRelease(canvasTextureView);
		SafeRelease(canvasTexture);
		SafeRelease(colorSampler);
		SafeRelease(renderTargetView);
		SafeRelease(swapChain);
		SafeRelease(deviceContext);
		SafeRelease(device);

	}

	


};

