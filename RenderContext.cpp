#include "stdafx.h"
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

ID3D11Device* RenderContext::GetDevice()
{
	return device.Get();
}

ID3D11DeviceContext* RenderContext::GetDC()
{
	return deviceContext.Get();
}

ID3D11RenderTargetView* RenderContext::GetRTV()
{
	return renderTargetView.Get();
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

	
	
}

void RenderContext::InitShaders()
{
}
