#pragma once
#include "Texture.h"
#include <wrl/client.h>
class D3D
{
public:
	static ID3D11Device* GetDevice();
	static ID3D11DeviceContext* GetDC();
	static ID3D11RenderTargetView* GetBB();
	static ID3D11RenderTargetView* GetRTV();
	static ID3D11ShaderResourceView* GetSRV();
	static ID3D11RenderTargetView* GetDisplayRTV();
	static ID3D11DepthStencilView* GetDisplayDSV();




	static HRESULT Present(UINT syncInterval, UINT flag);
	static float GetWndAspectRatio();
	static int GetWidth();
	static int GetHeight();
	static D3D11_VIEWPORT* GetViewport();

	static float GetDisplayAspectRatio();
	static void SetDisplaySize(int width, int height);
	static int GetDisplayWidth();
	static int GetDisplayHeight();



	static void Initialize(HWND window, int width, int height);
	static void ResizeBackBuffer(int width, int height);
	static void ResizeDisplay(int width, int height);


	void InitShaders();
	void CreateRenderTarget();
	void CleanupRenderTarget();
	void ResizeBuffer(int width, int height);
	void CreateDisplayResource(int width, int height);

private:
	static D3D* GetRenderContext();

	void Intialize_internal(HWND window, int width, int height);
	void UpdateBackbufferCopy();

	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;

	Microsoft::WRL::ComPtr< ID3D11RenderTargetView> copyRt;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> copyTx;

	ComPtr<ID3D11Texture2D> depthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> dss; 
	ComPtr<ID3D11DepthStencilView> dsv;

	std::shared_ptr<Texture> displayTexture;

	D3D11_VIEWPORT viewport;

	int width;
	int height = 0.1f;

	int displayWidth;
	int displayHeight = 0.1f;

};

