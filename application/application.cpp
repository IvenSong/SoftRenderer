#include"application.h"
#include <windowsx.h>

Application* Application::getInstance() {
	static Application instance;
	return &instance;
}

Application::Application() {}
Application::~Application() {
	if (mCanvasDC && mOldBmp) {
		SelectObject(mCanvasDC, mOldBmp);
	}

	if (mhBmp) {
		DeleteObject(mhBmp);
		mhBmp = nullptr;
	}

	if (mCanvasDC) {
		DeleteDC(mCanvasDC);
		mCanvasDC = nullptr;
	}

	if (mHwnd && mhDC) {
		ReleaseDC(mHwnd, mhDC);
		mhDC = nullptr;
	}
}

LRESULT CALLBACK Wndproc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	Application::getInstance()->handleMessage(hWnd, message, wParam, lParam);
	return(DefWindowProc(hWnd, message, wParam, lParam));
}

bool Application::initApplication(HINSTANCE hInstance, const uint32_t& width, const uint32_t& height) {
	mWidth = width;
	mHeight = height;

	registerWindowClass(hInstance);

	if (!createWindow(hInstance)) {
		return false;
	}


	mhDC = GetDC(mHwnd);
	if (!mhDC) {
		return false;
	}

	mCanvasDC = CreateCompatibleDC(mhDC);
	if (!mCanvasDC) {
		ReleaseDC(mHwnd, mhDC);
		mhDC = nullptr;
		return false;
	}

	BITMAPINFO bmpInfo{};
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	bmpInfo.bmiHeader.biWidth = mWidth;

	bmpInfo.bmiHeader.biHeight = mHeight;

	bmpInfo.bmiHeader.biPlanes = 1;

	bmpInfo.bmiHeader.biBitCount = 32;

	bmpInfo.bmiHeader.biCompression = BI_RGB;

	mhBmp = CreateDIBSection(mCanvasDC, &bmpInfo, DIB_RGB_COLORS, (void**)&mCanvasBuffer, 0, 0);
	if (!mhBmp || !mCanvasBuffer) {
		DeleteDC(mCanvasDC);
		mCanvasDC = nullptr;
		ReleaseDC(mHwnd, mhDC);
		mhDC = nullptr;
		return false;
	}

	mOldBmp = SelectObject(mCanvasDC, mhBmp);
	if (!mOldBmp || mOldBmp == HGDI_ERROR) {
		DeleteObject(mhBmp);
		mhBmp = nullptr;
		mCanvasBuffer = nullptr;
		DeleteDC(mCanvasDC);
		mCanvasDC = nullptr;
		ReleaseDC(mHwnd, mhDC);
		mhDC = nullptr;
		return false;
	}

	memset(mCanvasBuffer, 0, mWidth * mHeight * 4);

	return true;
}

ATOM Application::registerWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wndClass;

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	// re-paint window when height and width change
	wndClass.lpfnWndProc = Wndproc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = hInstance;		// application handle
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);// application icon
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);// mouse icon
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);// windows backgroud color
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = mWindowClassName;// windows class name
	wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // header of windows

	return RegisterClassExW(&wndClass);
}

BOOL Application::createWindow(HINSTANCE hInstance)
{
	mWindowInst = hInstance;


	auto dwExStyle = WS_EX_APPWINDOW;
	auto dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	RECT windowRect;
	windowRect.left = 0L;
	windowRect.top = 0L;
	windowRect.right = (long)mWidth;
	windowRect.bottom = (long)mHeight;
	AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	mHwnd = CreateWindowW(
		mWindowClassName,
		(LPCWSTR)"SoftRenderer",	// window head
		dwStyle,
		500,// x position
		500,// y position
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,// father window
		nullptr,// menu
		hInstance,
		nullptr); // extra parameters


	if (!mHwnd)
	{
		return FALSE;
	}

	ShowWindow(mHwnd, true);
	UpdateWindow(mHwnd);

	return TRUE;
}

bool Application::peekMessage() {
	MSG msg;
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return mAlive;
}

void Application::handleMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_KEYDOWN: {
		if (mInputHandler) {
			const uint32_t key = static_cast<uint32_t>(wParam);
			mInputHandler->onKeyDown(key);
		}
		break;
	}
	case WM_KEYUP: {
		if (mInputHandler) {
			const uint32_t key = static_cast<uint32_t>(wParam);
			mInputHandler->onKeyUp(key);
		}
		break;
	}
	case WM_RBUTTONDOWN: {
		if (mInputHandler) {
			const int x = GET_X_LPARAM(lParam);
			const int y = GET_Y_LPARAM(lParam);
			mInputHandler->onRMouseDown(x, y);
		}
		break;
	}
	case WM_RBUTTONUP: {
		if (mInputHandler) {
			const int x = GET_X_LPARAM(lParam);
			const int y = GET_Y_LPARAM(lParam);
			mInputHandler->onRMouseUp(x, y);
		}
		break;
	}
	case WM_MOUSEMOVE: {
		if (mInputHandler) {
			const int x = GET_X_LPARAM(lParam);
			const int y = GET_Y_LPARAM(lParam);
			mInputHandler->onMouseMove(x, y);
		}
		break;
	}
	case WM_CLOSE: {
		DestroyWindow(hWnd);
		break;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY: {
		if (mhDC) {
			ReleaseDC(hWnd, mhDC);
			mhDC = nullptr;
		}
		PostQuitMessage(0);
		mAlive = false;
		break;
	}
	}
}

void Application::show() {
	BitBlt(mhDC, 0, 0, mWidth, mHeight, mCanvasDC, 0, 0, SRCCOPY);
}
