#pragma once

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include "GraphicDefines.h"
#include "Sprite.h"

class Window
{
public:
	Window(HINSTANCE hInstance, int nCmdShow);

	~Window();

	int messageLoop();

private:
	HWND hWnd_ = 0;
	HINSTANCE hInstance_ = 0;
	Sprite* sprite_ = 0;
	const WCHAR defaultClassName_[12] = L"SpriteClass";
	const WCHAR defaultWindowName_[8] = L"Sprite";

	HWND initialize(HINSTANCE hInstance, int nCmdShow);

	ATOM registerClass(WNDCLASSEX wndClassEx, HINSTANCE hInstance, WNDPROC wndProc);

	static LRESULT CALLBACK windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
