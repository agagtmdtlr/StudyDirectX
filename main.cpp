#include "stdafx.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <Windows.h>
#include <memory>
#include <iostream>
#include <ImGuizmo.h>

#include "Example.h"
#include "Circle.h"
#include "RenderContext.h"

LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam , LPARAM lParam);

int main()
{
	const int width = 1280, height = 720;

	WNDCLASSEX wc = {
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0L,
		0L,
		GetModuleHandle(NULL),
		NULL,
		NULL,
		NULL,
		NULL,
		L"SeungLabGraphics", // lpszClassName, L-string
		NULL
	};

	RegisterClassEx(&wc);

	RECT wr = {0,0,width,height};
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		wc.lpszClassName,
		L"SeungLabGraphics Example",
		WS_OVERLAPPEDWINDOW,
		100, // 윈도우 좌측 상단의 x좌표
		100, // 윈도우 좌측 상단의 y좌표
		wr.right - wr.left, // window horizontal arrow pixel rate
		wr.bottom - wr.top, // window vertical arrow pixel rate
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);

	auto example = std::make_unique<Example>(hwnd, width, height);
	//RenderContext* renderContext = RenderContext::GetRenderContext();
	//renderContext->Initialize(hwnd, width, height);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImVec2(width, height);
	ImGui::StyleColorsLight();

	ImGuiContext* imguiContext = ImGui::GetCurrentContext();
	ImGuizmo::SetImGuiContext(imguiContext);


	// Setup Platform/ Renderer backedns
	ImGui_ImplDX11_Init(example->device, example->deviceContext);
	ImGui_ImplWin32_Init(hwnd);

	//Main message loop
	MSG msg ={};
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame(); 
			ImGui_ImplWin32_NewFrame();
			ImGuizmo::BeginFrame();


			ImGui::NewFrame();
			ImGui::Begin("Option");
			//ImGui::Text("ElapsedTime %lf", 1.0f / example->elapsedTime);
			ImGui::InputInt("Sumper Sampling Level",&example->raytracer.supersmaplingLevel);
			ImGui::End();
			ImGui::Render();

			example->Update();
			example->Render();


			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// switch the back buffer and the front buffer
			example->swapChain->Present(1,0);
		}
	} // while WM_QUIT

	//Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	example->Clean();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);

	return 0;
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Windows procedure
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(ImGui_ImplWin32_WndProcHandler(hWnd,msg,wParam,lParam))
		return true;

	switch (msg)
	{
	case WM_SIZE:
		// Reeset and resize swapchain
		return 0;
	case WM_SYSCOMMAND:
		if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
			return 0;
		break;
	case WM_MOUSEMOVE:
		//std::cout << "Mouse " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		break;
	case WM_LBUTTONUP:
		//std::cout << "WM_LBUTTONUP Left mouse button" << std::endl;
		break;
	case WM_RBUTTONUP:
		//std::cout << "WM_RBUTTONUP Right mouse button" << std::endl;
		break;
	case WM_KEYDOWN:
		//std::cout << "WM_KEYDOWN " << (int)wParam << std::endl;
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam,lParam);
}