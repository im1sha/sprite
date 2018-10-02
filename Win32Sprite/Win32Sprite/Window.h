#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

class Window
{
public:

	Window(HINSTANCE hInstance, int nCmdShow);

	~Window();

	int messageLoop();	
	
private:
	HWND hwnd_;
	MSG msg_;
	WNDCLASS wc_;
	const wchar_t className_[6] = L"Shape"; 

	BOOL show();

	WORD registerClass(HINSTANCE hInstance);

	HWND create(HINSTANCE hInstance);

	HWND getWindowHandle();

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};