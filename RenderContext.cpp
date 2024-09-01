#include "RenderContext.h"

#include <iostream>


RenderContext* RenderContext::GetRenderContext()
{
	static RenderContext* renderContext = nullptr;
	if (renderContext == nullptr)
	{
		renderContext = new RenderContext();
	}
	return renderContext;
}

void RenderContext::Initialize(HWND window, int width, int height)
{
	this->width = width;
	this->height = height;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	{
		DXGI_MODE_DESC& modeDesc = swapChainDesc.BufferDesc;
		modeDesc.Width = width;
		modeDesc.Height = height;
		modeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;// use 32-bit color
		modeDesc.RefreshRate.Numerator = 60;
		modeDesc.RefreshRate.Denominator = 1;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 
		swapChainDesc.OutputWindow = window;
		swapChainDesc.SampleDesc.Count = 1; // how many multisamples;
		swapChainDesc.Windowed = TRUE; // windowed/full-screen mode
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;// allow full-screen switching
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT createDeviceFlags = 0;
	const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
	if (FAILED(D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		NULL,
		deviceContext.GetAddressOf())))
	{
		std::cout << "D3D11CrateDeviceAndSwapChain() error" << std::endl;
	}

	//CreateRenderTarget
	ID3D11Texture2D* pBackBuffer;
	swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	if (pBackBuffer)
	{
		device->CreateRenderTargetView(pBackBuffer, NULL, renderTargetView.GetAddressOf());
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
}

void RenderContext::InitShaders()
{
}

void RenderContext::Render()
{
	
}
