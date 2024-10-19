#pragma once
#include <Windows.h>


class System
{
public:
	System();
	~System();
	
	void Initialize();
	void InitializeWindow(int width, int height);
	void InitializeUI();

	void Run();
	void Shutdown();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:

	HWND hwnd;
	WNDCLASSEX wc;

	unique_ptr<class Renderer> renderer;
	unique_ptr<class Application> application;
	unique_ptr<class Input> input;
	unique_ptr<class ControllerManager> ui;

	int resizeWidth = 0;
	int resizeHeight = 0;
};

static System* systemHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

