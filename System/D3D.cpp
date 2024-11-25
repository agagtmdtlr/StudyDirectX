#include "stdafx.h"
#include "D3D.h"
#include "RenderCommon.h"
#include <memory>
#include <d3d11.h>
#include <imgui.h>
#include <dxgi.h>

D3D* D3D::GetRenderContext()
{
	static D3D* renderContext = nullptr;
	if (renderContext == nullptr)
	{
		renderContext = new D3D();
	}
	return renderContext;
}

void D3D::Intialize_internal(HWND window, int width, int height)
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
		swapChain.GetAddressOf(),
		device.GetAddressOf(),
		NULL,
		deviceContext.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "D3D11CrateDeviceAndSwapChain() error" << std::endl;
	}

	//CreateRenderTarget
	ID3D11Texture2D* pBackBuffer;

	ID3D11Device* device = D3D::GetDevice();

	{
		DXGI_SWAP_CHAIN_DESC desc;
		swapChain->GetDesc(&desc);
		swapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
	}

	CleanupRenderTarget();
	CreateRenderTarget();
}

void D3D::UpdateBackbufferCopy()
{
	return;
	ComPtr<ID3D11Resource> srcRrc;
	ComPtr<ID3D11Resource> dstRrc;

	srv->GetResource(srcRrc.GetAddressOf());
	copyTx->GetResource(dstRrc.GetAddressOf());

	ComPtr<IDXGIKeyedMutex> mutexA;
	HRESULT hr = srcRrc.As(&mutexA);
	{
		//mutexA->AcquireSync(1,0);
		deviceContext->CopyResource(dstRrc.Get(), srcRrc.Get());
		//mutexA->ReleaseSync(0);
	}


}

ID3D11Device* D3D::GetDevice()
{
	return D3D::GetRenderContext()->device.Get();
}

ID3D11DeviceContext* D3D::GetDC()
{
	return D3D::GetRenderContext()->deviceContext.Get();
}

ID3D11RenderTargetView* D3D::GetBB()
{
	return D3D::GetRenderContext()->renderTargetView.Get();
}

ID3D11RenderTargetView* D3D::GetRTV()
{
	return D3D::GetRenderContext()->renderTargetView.Get();
	//return D3D::GetRenderContext()->copyRt.Get();	
}


ID3D11ShaderResourceView* D3D::GetSRV()
{
	//D3D::GetRenderContext()->UpdateBackbufferCopy();
	return D3D::GetRenderContext()->copyTx.Get();
}

ID3D11RenderTargetView* D3D::GetDisplayRTV()
{
	return D3D::GetRenderContext()->copyRt.Get();;
}

ID3D11DepthStencilView* D3D::GetDisplayDSV()
{
	return D3D::GetRenderContext()->dsv.Get();
}



HRESULT D3D::Present(UINT syncInterval, UINT flag)
{
	return D3D::GetRenderContext()->swapChain->Present(syncInterval, flag);
}

float D3D::GetWndAspectRatio()
{
	float aspectRatio = (float)D3D::GetRenderContext()->width / D3D::GetRenderContext()->height;
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

D3D11_VIEWPORT* D3D::GetViewport()
{
	return &(D3D::GetRenderContext()->viewport);
}

float D3D::GetDisplayAspectRatio()
{
	float aspectRatio = (float)D3D::GetRenderContext()->displayWidth / D3D::GetRenderContext()->displayHeight;
	return aspectRatio;
}

void D3D::SetDisplaySize(int width, int height)
{
	D3D::GetRenderContext()->displayWidth = width;
	D3D::GetRenderContext()->displayHeight = height;
}

int D3D::GetDisplayWidth()
{
	return D3D::GetRenderContext()->displayWidth;
}

int D3D::GetDisplayHeight()
{
	return D3D::GetRenderContext()->displayHeight;
}

void D3D::Initialize(HWND window, int width, int height)
{
	auto rc = D3D::GetRenderContext();

	rc->Intialize_internal(window, width, height);

}

void D3D::ResizeBackBuffer(int width, int height)
{
	auto rc = D3D::GetRenderContext();
	rc->ResizeBuffer(width, height);
}

void D3D::ResizeDisplay(int width, int height)
{
	D3D::SetDisplaySize(width,height);
	D3D::GetRenderContext()->CreateDisplayResource(width, height);
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

void D3D::ResizeBuffer(int width, int height)
{

	CleanupRenderTarget();
	swapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	CreateRenderTarget();

	this->width = width;
	this->height = height;
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
}

void D3D::CreateDisplayResource(int width, int height)
{
	copyTx.Reset();
	copyRt.Reset();

	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	vector<int> v(width * height * 4);
	data.pSysMem = v.data();
	data.SysMemPitch = 4 * width;
	data.SysMemSlicePitch = 1;

	ComPtr<ID3D11Texture2D> tx;
	HRESULT hr = device->CreateTexture2D(&desc, &data, tx.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "failed" << endl;
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC srvdesc;
	ZeroMemory(&srvdesc, sizeof(srvdesc));
	srvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvdesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvdesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(tx.Get(), &srvdesc, copyTx.GetAddressOf());
	if (FAILED(hr))
		std::cout << "failed" << endl;

	D3D11_RENDER_TARGET_VIEW_DESC rtvdesc;
	ZeroMemory(&rtvdesc, sizeof(rtvdesc));
	rtvdesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvdesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvdesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(tx.Get(), &rtvdesc, copyRt.GetAddressOf());
	if (FAILED(hr))
		std::cout << "failed" << endl;


	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = displayWidth;
	depthBufferDesc.Height = displayHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = device->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer.GetAddressOf());
	if(FAILED(hr))
		cout << "failed create depth stencil buffer" << endl;


	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	// Set up the description of the stencil state
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dssDesc.StencilEnable = true;
	dssDesc.StencilReadMask = 0xFF;
	dssDesc.StencilWriteMask = 0xFF;

	//Stencil operations if pixel is front-facing
	dssDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilDepthFailOp =D3D11_STENCIL_OP_INCR;
	dssDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	//Stencil operations if pixel is back-facing
	dssDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	dssDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dssDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	hr = device->CreateDepthStencilState(&dssDesc,dss.GetAddressOf());
	if( FAILED(hr))
		cout << "failed create display dss" << endl;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), &dsvDesc, dsv.GetAddressOf());
	if (FAILED(hr))
		cout << "failed create display dsv" << endl;
}
