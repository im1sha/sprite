#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

wchar_t ClassName[] = L"Shape";

class Window
{
public:
	HWND hwnd;              
	MSG msg;              
	WNDCLASS wc;			

	WORD Register(HINSTANCE hinstance) 
	{
		wc.hInstance = hinstance;
		wc.lpszClassName = ClassName;
		wc.lpfnWndProc = WindowProc;                
		wc.style = CS_HREDRAW | CS_VREDRAW;     
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);    
		wc.hCursor = LoadCursor(NULL, IDC_ARROW); 
		wc.lpszMenuName = NULL;       
		wc.cbClsExtra = 0;            
		wc.cbWndExtra = 0;            
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		WORD successful = RegisterClass(&wc);
		return successful;
	}

	HWND Create(HINSTANCE hInstance)
	{
		hwnd = CreateWindow(
			ClassName, 
			L"WindowName",
			WS_OVERLAPPEDWINDOW,         
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
			HWND_DESKTOP, 
			NULL,         
			hInstance,
			NULL
		);   
		return hwnd;
	}

	BOOL Show() 
	{
		return ShowWindow(hwnd, SW_SHOW); 
	}
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	MSG msg;
	Window window;

	window.Register(hInstance);

	if (window.Create(hInstance) == NULL)
	{
		return -1;
	}

	window.Show();

	BOOL result;
	while (result = GetMessage(&msg, NULL, 0, 0))
	{
		if (result == -1)
		{
			// handle error
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}