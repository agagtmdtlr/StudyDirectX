#include "stdafx.h"
#include "SystemClass.h"
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <iostream>
#include <ImGuizmo.h>
#include <memory>


#include "D3D.h"
#include "InputClass.h"
#include "Application.h"
#include "Renderer.h"
#include "UIManager.h" 

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

SystemClass::SystemClass()
{
}

SystemClass::~SystemClass()
{
}

void SystemClass::Initialize()
{
    const int width = 1280, height = 720;
    InitializeWindow(width, height);

	D3D::Initialize(hwnd, width, height);

	input = make_unique<InputClass>();
	input->Initialize();

	application = make_unique<Application>();

	renderer = std::make_unique<Renderer>();
	renderer->Initialize(width, height);

	InitializeUI();
}

void SystemClass::InitializeWindow(int width, int height)
{
	wc = {
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

	RECT wr = { 0,0,width,height };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindow(
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
}

void SystemClass::InitializeUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize.x = D3D::GetWidth();
	io.DisplaySize.y = D3D::GetHeight();
	ImGui::StyleColorsLight();

	ImGuiContext* imguiContext = ImGui::GetCurrentContext();
	ImGuizmo::SetImGuiContext(imguiContext);

	// Setup Platform/ Renderer backedns
	ImGui_ImplDX11_Init(D3D::GetDevice(), D3D::GetDC());
	ImGui_ImplWin32_Init(hwnd);

	ui = make_unique<UIManager>(application.get(), renderer.get());
	ui->Initialize();
}

void SystemClass::Run()
{
	//Main message loop
	MSG msg = {};
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


			ImGui::NewFrame();

			ImGui::Begin("Option");
			//ImGui::Text("ElapsedTime %lf", 1.0f / example->elapsedTime);
			//ImGui::InputInt("Sumper Sampling Level",&example->raytracer.supersmaplingLevel);

			ImGuizmo::BeginFrame();
			

			//example->Update();
			//example->Render();

			renderer->Update();
			renderer->Render();

			ui->Render();

			ImGui::End();
			ImGui::Render();

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			// switch the back buffer and the front buffer
			D3D::Present(1, 0);
			//example->swapChain->Present(1,0);
		}
	} // while WM_QUIT
}

void SystemClass::Shutdown()
{
	//Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//example->Clean();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

LRESULT SystemClass::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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
	case WM_KEYUP:
		break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;

	switch (msg)
	{
		case WM_DESTROY:
		{
			::PostQuitMessage(0);
			return 0;
		}
		case WM_CLOSE:
		{
			::PostQuitMessage(0);
			return 0;
		}

		default :
		{
			return systemHandle->MessageHandler(hwnd, msg, wparam, lparam);
		}

	}
}
