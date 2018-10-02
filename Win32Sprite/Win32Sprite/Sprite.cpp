#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <cmath>
#include <ctime>

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

const wchar_t CLASS_NAME[] = L"Shape";

class Window
{
private:
	HWND hwnd;              
	MSG msg;              
	WNDCLASS wc;

public:
	WORD Register(HINSTANCE hinstance) 
	{
		wc.hInstance = hinstance;
		wc.lpszClassName = CLASS_NAME;
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
			CLASS_NAME, 
			L"Win32Sprite",
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

	HWND GetWindowHandle() 
	{
		return hwnd;
	}
};

class Sprite 
{
private:
	const double pi = 3.14;
	const int semicircleInDegrees = 180;
	const int circleInDegrees = 360;
	const int defaultRadius = 20;
	const double defaultSpeed = 10.0;

	RECT spriteRect;
	int x;
	int y;
	int radius;     
	double speedX;
	double speedY;

public:
	void CreateMoveParameters() 
	{	
		radius = defaultRadius;

		srand(time(NULL));
		double initValue = ((rand() % circleInDegrees) - semicircleInDegrees) * pi / semicircleInDegrees;

		speedX = defaultSpeed * cos(initValue);
		speedY = defaultSpeed * sin(initValue);
	}

	void Invalidate(HWND &hwnd)
	{
		InvalidateRect(hwnd, &spriteRect, true);

		x += speedX; 
		y += speedY;

		RECT clientRect;
		GetClientRect(hwnd, &clientRect);
		int width = clientRect.right;
		int height = clientRect.bottom;

		if ((x >= width - radius) || (x < radius))
		{
			speedX = -speedX;
		}
		if ((y > height - radius) || (y < radius))
		{
			speedY = -speedY;
		}

		spriteRect.left = x - radius - 1;
		spriteRect.top = y - radius - 1;
		spriteRect.right = x + radius + 1;
		spriteRect.bottom = y + radius + 1;

		InvalidateRect(hwnd, &spriteRect, false); 
	}

	void Create(HWND hwnd)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		HBRUSH brush; 
		HBRUSH oldBrush;

		hdc = BeginPaint(hwnd, &ps);

		brush = CreateSolidBrush(RGB(185, 244, 66));
		oldBrush = (HBRUSH)SelectObject(hdc, brush);
		Ellipse(hdc, x - radius, y - radius, x + radius, y + radius); 

		SelectObject(hdc, oldBrush);
		DeleteObject(brush); 

		EndPaint(hwnd, &ps);
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
			return -1;
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
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);	
		return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}