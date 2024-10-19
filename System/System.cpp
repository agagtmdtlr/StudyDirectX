#include "stdafx.h"
#include "System.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <iostream>
#include <ImGuizmo.h>
#include <memory>


#include "D3D.h"
#include "Input.h"
#include "Application.h"
#include "Renderer.h"
#include "ControllerManager.h" 

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

System::System()
{
}

System::~System()
{
}

void System::Initialize()
{
	systemHandle = this;
    const int width = 1920, height = 1080;
    InitializeWindow(width, height);

	D3D::Initialize(hwnd, width, height);

	input = make_unique<Input>();
	input->Initialize();

	renderer = std::make_unique<Renderer>();
	renderer->Initialize(width, height);

	InitializeUI();

	application = make_unique<Application>(ui.get());
	application->Initialize();
}

void System::InitializeWindow(int width, int height)
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
		0, // 윈도우 좌측 상단의 x좌표
		0, // 윈도우 좌측 상단의 y좌표
		wr.right - wr.left, // window horizontal arrow pixel rate
		wr.bottom - wr.top, // window vertical arrow pixel rate
		NULL,
		NULL,
		wc.hInstance,
		NULL
	);

	ShowWindow(hwnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hwnd);
}

void System::InitializeUI()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsLight();

	ImGuiContext* imguiContext = ImGui::GetCurrentContext();
	ImGuizmo::SetImGuiContext(imguiContext);

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/ Renderer backedns
	ImGui_ImplDX11_Init(D3D::GetDevice(), D3D::GetDC());
	ImGui_ImplWin32_Init(hwnd);

	ui = make_unique<ControllerManager>(renderer.get());
	ui->Initialize();
}

void System::Run()
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
			if (resizeWidth != 0 && resizeHeight != 0)
			{
				D3D::ResizeBackBuffer(resizeWidth, resizeHeight);
				resizeWidth = 0;
				resizeHeight = 0;
			}

			// Start the Dear ImGui frame
			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();

			ImGui::NewFrame();
			ImGuizmo::BeginFrame();

			//ImGui::ShowDemoWindow();
			ImGui::BeginMainMenuBar();
			if (ImGui::MenuItem("File"))
			{
				if (ImGui::MenuItem("Open", "Ctrl+O"))
				{
					
				}

			}
			ImGui::EndMainMenuBar();

			//ImGui::Text("ElapsedTime %lf", 1.0f / example->elapsedTime);
			//ImGui::InputInt("Sumper Sampling Level",&example->raytracer.supersmaplingLevel);

			//example->Update();
			//example->Render();


			renderer->Update();
			renderer->Render();

			ui->UpdateControllerManager();
			ui->Render();


			//TODO:: DockSpaceOverViewport()


			ImGui::Render();

			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
			{
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
			}

			// switch the back buffer and the front buffer
			D3D::Present(1, 0);
			//example->swapChain->Present(1,0);
		}
	} // while WM_QUIT
}

void System::Shutdown()
{
	//Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	//example->Clean();
	DestroyWindow(hwnd);
	UnregisterClass(wc.lpszClassName, wc.hInstance);
}

LRESULT System::MessageHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		// Reeset and resize swapchain
		std::cout << "Resize " << LOWORD(lParam) << " " << HIWORD(lParam) << std::endl;
		resizeWidth = LOWORD(lParam);
		resizeHeight = HIWORD(lParam);
		return 0;
	}		
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
			if(systemHandle != nullptr )
				return systemHandle->MessageHandler(hwnd, msg, wparam, lparam);
			else
				return ::DefWindowProc(hwnd, msg, wparam, lparam);;
		}

	}
}
