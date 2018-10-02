

#include "Window.h"


Window::Window(HINSTANCE hInstance, int nCmdShow)
{
	registerClass(hInstance);
	create(hInstance);
	show();
}

Window::~Window()
{

}

WORD Window::registerClass(HINSTANCE hInstance)
{
	wc_.hInstance = hInstance;
	wc_.lpszClassName = className_;
	wc_.lpfnWndProc = windowProc;
	wc_.style = CS_HREDRAW | CS_VREDRAW;
	wc_.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc_.hCursor = LoadCursor(0, IDC_ARROW);
	wc_.lpszMenuName = 0;
	wc_.cbClsExtra = 0;
	wc_.cbWndExtra = 0;
	wc_.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	WORD successful = RegisterClass(&wc_);
	return successful;
}

HWND Window::create(HINSTANCE hInstance)
{
	hwnd_ = CreateWindow(
		className_,
		L"Win32Sprite",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_DESKTOP,
		0,
		hInstance,
		0
	);
	return hwnd_;
}

BOOL Window::show()
{
	return ShowWindow(hwnd_, SW_SHOW);
}

HWND Window::getWindowHandle()
{
	return hwnd_;
}

int Window::messageLoop()
{
	MSG msg;
	BOOL result;
	while (result = GetMessage(&msg, 0, 0, 0))
	{
		if (result == -1) {
			return -1;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK Window::windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

