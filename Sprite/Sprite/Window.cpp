#include "Window.h"

Window::Window(HINSTANCE hInstance, int nCmdShow)
{
	this->hInstance_ = hInstance;
	HWND hWnd = initialize(hInstance, nCmdShow);
	this->hWnd_ = hWnd;
	this->sprite_ = new Sprite(hWnd, hInstance);
}

Window::~Window()
{
	delete this->sprite_;
}

int Window::messageLoop()
{
	MSG msg;
	BOOL result;

	while (result = GetMessage(&msg, 0, 0, 0))
	{
		if (-1 == result)
		{
			// HANDLE THIS ERROR
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

HWND Window::initialize(HINSTANCE hInstance, int nCmdShow)
{
	WNDCLASSEX wndClassEx = { };
	registerClass(wndClassEx, hInstance, Window::windowProc);
	HWND hWnd = CreateWindow(defaultClassName_, defaultWindowName_, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);

	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)this);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	return hWnd;
}

ATOM Window::registerClass(WNDCLASSEX wndClassEx, HINSTANCE hInstance, WNDPROC wndProc)
{
	wndClassEx.cbSize = sizeof(WNDCLASSEX);
	wndClassEx.style = CS_DBLCLKS;
	wndClassEx.lpfnWndProc = wndProc;
	wndClassEx.cbClsExtra = 0;
	wndClassEx.cbWndExtra = 0;
	wndClassEx.hInstance = hInstance;
	wndClassEx.hIcon = 0;
	wndClassEx.hCursor = LoadCursor(0, IDC_ARROW);
	wndClassEx.hbrBackground = CreateSolidBrush(DEFAULT_BACKGROUND_COLOR);
	wndClassEx.lpszMenuName = 0;
	wndClassEx.lpszClassName = defaultClassName_;
	wndClassEx.hIconSm = 0;
	return RegisterClassEx(&wndClassEx);
}

LRESULT CALLBACK Window::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	Window* window = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	Sprite* sprite = 0;
	if (window != 0)
	{
		sprite = window->sprite_;
	}

	switch (message)
	{
		if (sprite != 0)
		{
	case WM_SPRITE_DEFAULT_IMAGE:
	{
		sprite->loadDefault(lParam);
		break;
	}
	case WM_SPRITE_LOADING:
	{
		sprite->load();
		break;
	}
	case WM_SPRITE_UPDATING:
	{
		sprite->update();
		break;
	}
	case WM_LBUTTONDOWN:
	{
		sprite->respondOnLeftButtonClick(lParam);
		break;
	}
	case WM_LBUTTONUP:
	{
		sprite->respondOnLeftButtonRelease();
		break;
	}
	case WM_MOUSEMOVE:
	{
		sprite->respondOnDragging(wParam, lParam);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		sprite->respondOnRightButtonClick(lParam);
		break;
	}
	case WM_TIMER:
	{
		sprite->respondOnTimer();
		break;
	}
	case WM_KEYDOWN:
	{
		sprite->respondOnKeyPress(wParam);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		sprite->respondOnScrolling(wParam);
		break;
	}
		}
	case WM_SIZE:
	case WM_PAINT:
	{
		if (sprite != 0)
		{
			sprite->update();
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	default:
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	}
	return 0;
}


