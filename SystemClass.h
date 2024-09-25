#pragma once
#include "InputClass.h"
#include "ApplicationClass.h"
#include "RenderDemo.h"

#include <Windows.h>


class SystemClass
{
public:
	SystemClass();
	~SystemClass();
	
	void Initialize();
	void InitializeWindow(int width, int height);

	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	HWND hwnd;
	unique_ptr<RenderDemo> demo;
	unique_ptr<ApplicationClass> application;
	unique_ptr<InputClass> input;
	WNDCLASSEX wc;
};

static SystemClass* systemHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

