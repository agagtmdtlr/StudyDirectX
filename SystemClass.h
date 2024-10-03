#pragma once
#include "InputClass.h"
#include "ApplicationClass.h"
#include "Renderer.h"

#include <Windows.h>


class SystemClass
{
public:
	SystemClass();
	~SystemClass();
	
	void Initialize();
	void InitializeWindow(int width, int height);
	void InitializeUI();

	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	HWND hwnd;
	WNDCLASSEX wc;

	unique_ptr<Renderer> renderer;
	unique_ptr<ApplicationClass> application;
	unique_ptr<InputClass> input;
	unique_ptr<class UIManager> ui;
};

static SystemClass* systemHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

