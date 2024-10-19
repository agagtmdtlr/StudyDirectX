#pragma once


class D3D
{	
public:
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDC();
	static ID3D11RenderTargetView* GetRTV();
	static ID3D11ShaderResourceView* GetSRV();

	
	static HRESULT Present(UINT syncInterval, UINT flag);
	static float GetWndAspectRatio();
	static int GetWidth();
	static int GetHeight();


	static void Initialize(HWND window, int width, int height);
	static void ResizeBackBuffer(int width, int height);
	void InitShaders();
	void CreateRenderTarget();
	void CleanupRenderTarget();

private:
	static D3D* GetRenderContext();
	


	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11DeviceContext> deviceContext;
	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11ShaderResourceView> srv;

	int width;
	int height;

};

