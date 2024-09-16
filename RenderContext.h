#pragma once

#include <memory>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include "RenderCommon.h"
#include <wrl/client.h>
#include "Mesh.h"


class RenderContext
{	
public:
	static RenderContext* GetRenderContext();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDC();
	ID3D11RenderTargetView* GetRTV();


	void Initialize(HWND window, int width, int height);
	void InitShaders();


private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;

	ComPtr<ID3D11RenderTargetView> renderTargetView; // swap chain
	

	int width, height;

};
