#pragma once

#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "Sprite.h"
#include "Handler.h"

class Window
{
public:
	
	Window(HINSTANCE hInstance, int nCmdShow);

	~Window();

	int messageLoop();	
	
private:
	const wchar_t className_[6] = L"Shape"; 
	const wchar_t windowName_[12] = L"Win32Sprite";
	
	HWND hwnd_;
	WNDCLASS wc_;

	Sprite* sprite_;
	Handler* handler_;
	
	BOOL show();

	WORD registerClass(HINSTANCE hInstance);

	HWND create(HINSTANCE hInstance);

	void BoundAction(POINT p);

	static LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};