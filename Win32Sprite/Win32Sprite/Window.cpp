

#include "Window.h"


Window::Window(HINSTANCE hInstance, int nCmdShow)
{
	registerClass(hInstance);
	
	sprite_ = new Sprite();
	handler_ = new Handler(sprite_);

	create(hInstance);
	show();
}

Window::~Window()
{
	delete handler_;
	delete sprite_;
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
		windowName_,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		HWND_DESKTOP,
		0,
		hInstance,
		0
	);
	return hwnd_;
}

void Window::BoundAction(POINT p)
{	
	if (sprite_->x_ < sprite_->radius_) {
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ += 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ += 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ += 14;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
	}
	if (sprite_->y_ < sprite_->radius_) {
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ += 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ += 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ += 14;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
	}
	if (sprite_->x_ >= p.x - sprite_->radius_) {
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ -= 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ -= 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->x_ -= 14;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
	}
	if (sprite_->y_ > p.y - sprite_->radius_) {
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ -= 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ -= 5;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
		InvalidateRect(hwnd_, &sprite_->spriteRect_, true);
		sprite_->y_ -= 14;
		InvalidateRect(hwnd_, &sprite_->spriteRect_, false);
	}
}

BOOL Window::show()
{
	return ShowWindow(hwnd_, SW_SHOW);
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


// redirect to HANDLER CLASS here
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

