#pragma once
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

	unique_ptr<class Renderer> renderer;
	unique_ptr<class Application> application;
	unique_ptr<class InputClass> input;
	unique_ptr<class UIManager> ui;
};

static SystemClass* systemHandle = nullptr;
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

