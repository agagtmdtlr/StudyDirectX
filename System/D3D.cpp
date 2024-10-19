#include "stdafx.h"
#include "D3D.h"
#include "RenderCommon.h"
#include <memory>


D3D* D3D::GetRenderContext()
{
	static D3D* renderContext = nullptr;
	if (renderContext == nullptr)
	{
		renderContext = new D3D();
	}
	return renderContext;
}

ID3D11Device* D3D::GetDevice()
{
	return D3D::GetRenderContext()->device.Get();
}

ID3D11DeviceContext* D3D::GetDC()
{
	return D3D::GetRenderContext()->deviceContext.Get();
}

ID3D11RenderTargetView* D3D::GetRTV()
{
	return D3D::GetRenderContext()->renderTargetView.Get();
}

ID3D11ShaderResourceView* D3D::GetSRV()
{
	return D3D::GetRenderContext()->srv.Get();
}



HRESULT D3D::Present(UINT syncInterval, UINT flag)
{
	return D3D::GetRenderContext()->swapChain->Present(syncInterval, flag);
}

float D3D::GetWndAspectRatio()
{
	float aspectRatio = (float) D3D::GetRenderContext()->width / D3D::GetRenderContext()->height;
	return aspectRatio;
}

int D3D::GetWidth()
{
	return D3D::GetRenderContext()->width;
}

int D3D::GetHeight()
{
	return D3D::GetRenderContext()->height;
}

void D3D::Initialize(HWND window, int width, int height)
{
	auto rc = D3D::GetRenderContext();
	rc->width = width;
	rc->height = height;

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
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // 
		swapChainDesc.OutputWindow = window;
		swapChainDesc.SampleDesc.Count = 1; // how many multisamples;
		swapChainDesc.Windowed = TRUE; // windowed/full-screen mode
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;// allow full-screen switching
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	}

	UINT createDeviceFlags = 0;
	const D3D_FEATURE_LEVEL featureLevelArray[1] = { D3D_FEATURE_LEVEL_11_0 };
	HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		createDeviceFlags,
		featureLevelArray,
		1,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		rc->swapChain.GetAddressOf(),
		rc->device.GetAddressOf(),
		NULL,
		rc->deviceContext.GetAddressOf());
	if ( FAILED(hr) )
	{
		std::cout << "D3D11CrateDeviceAndSwapChain() error" << std::endl;
	}

	//CreateRenderTarget
	ID3D11Texture2D* pBackBuffer;

	ID3D11Device* device = D3D::GetDevice();

	{
		DXGI_SWAP_CHAIN_DESC desc;
		rc->swapChain->GetDesc(&desc);
		rc->swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	}
	
	if (pBackBuffer)
	{
		device->CreateRenderTargetView(pBackBuffer, NULL, rc->renderTargetView.GetAddressOf());
		pBackBuffer->Release();

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		hr = device->CreateShaderResourceView(pBackBuffer, NULL, rc->srv.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "failed create srv for backbuffer" << std::endl;
		}
	}
}

void D3D::ResizeBackBuffer(int width, int height)
{
	auto rc = D3D::GetRenderContext();

	rc->CleanupRenderTarget();
	rc->swapChain->ResizeBuffers(0,width,height, DXGI_FORMAT_R8G8B8A8_UNORM,0);
	rc->CreateRenderTarget();

	rc->width = width;
	rc->height = height;
}

void D3D::InitShaders()
{
}

void D3D::CreateRenderTarget()
{
	//CreateRenderTarget
	ID3D11Texture2D* pBackBuffer;

	ID3D11Device* device = D3D::GetDevice();

	{
		DXGI_SWAP_CHAIN_DESC desc;
		swapChain->GetDesc(&desc);
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));

	}
	HRESULT hr;
	if (pBackBuffer)
	{
		device->CreateRenderTargetView(pBackBuffer, NULL, renderTargetView.GetAddressOf());
		pBackBuffer->Release();

		D3D11_SHADER_RESOURCE_VIEW_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		hr = device->CreateShaderResourceView(pBackBuffer, NULL, srv.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "failed create srv for backbuffer" << std::endl;
		}
	}
	else
	{
		std::cout << "CreateRenderTargetView() error" << std::endl;
		exit(-1);
	}
}

void D3D::CleanupRenderTarget()
{
	renderTargetView.Reset();
	srv.Reset();
}