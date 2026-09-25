#pragma once
#include "base.h"
#include "inputHandler.h"
#include<Windows.h>

#define app Application::getInstance()

class Application {
public:
	static Application* getInstance();
	Application();
	~Application();

	bool initApplication(HINSTANCE hInstance, const uint32_t& width = 800, const uint32_t& height = 600);

	void handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	// every loop call it in order to peek messages
	bool peekMessage();

	void show();
	void setInputHandler(InputHandler* handler) { mInputHandler = handler; }

	uint32_t getWidth() const { return mWidth; }
	uint32_t getHeight() const { return mHeight; }
	void* getCanvas() const { return mCanvasBuffer; }

private:
	BOOL createWindow(HINSTANCE hInstance);
	ATOM registerWindowClass(HINSTANCE hInstance);

private:
	InputHandler* mInputHandler{ nullptr };

	bool		mAlive{ true };

	HINSTANCE	mWindowInst{ nullptr };
	WCHAR		mWindowClassName[100] = L"AppWindow";
	HWND		mHwnd{ nullptr };

	int			mWidth = 800;
	int			mHeight = 600;

	HDC mhDC{ nullptr }; // main DC in current window
	HDC mCanvasDC{ nullptr }; // compatible DC
	HBITMAP mhBmp{ nullptr }; // mCanvasDC bitmap 
	HGDIOBJ mOldBmp{ nullptr }; // mCanvasDC selected before
	void* mCanvasBuffer{ nullptr }; // mhBmp start pointer
};
