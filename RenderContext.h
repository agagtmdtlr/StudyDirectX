#pragma once


class D3D
{	
public:
	static D3D* GetRenderContext();
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDC();
	static ID3D11RenderTargetView* GetRTV();
	static HRESULT Present(UINT syncInterval, UINT flag);
	static float GetWndAspectRatio();
	static int GetWidth();
	static int GetHeight();


	void Initialize(HWND window, int width, int height);
	void InitShaders();


private:
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;

	int width;
	int height;

};

