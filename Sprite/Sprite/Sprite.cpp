#pragma once

#include "Sprite.h"

Sprite::Sprite(HWND hWnd, HINSTANCE hInstanse)
{
	this->hWnd_ = hWnd;
	requestDefaultImageLoading(hInstanse);
}

Sprite::~Sprite()
{
	if (isAnimated_)
	{
		KillTimer(hWnd_, TIMER_NO);
	}
}

void Sprite::initializeSprite()
{
	position_ = { 0, 0 };
	angle_ = 0;
	reflected_ = TransformationMatrix::Reflection{ false, false };
	imageSize_ = getImageSize(image_);
}

bool Sprite::load()
{
	bool successful = loadImage();
	if (successful)
	{
		initializeSprite();
		requestImageUpdating();
	}
	else
	{
		MessageBox(hWnd_, L"Error while loading image", L"Error", MB_OK | MB_ICONERROR);
	}
	return successful;
}

bool Sprite::loadDefault()
{
	image_ = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1));
	bool successful = (image_ != 0);
	if (successful)
	{
		initializeSprite();
		requestImageUpdating();
	}
	return successful;
}

bool Sprite::update()
{
	fillBackground(DEFAULT_BACKGROUND_COLOR);
	return placeSprite();
}

void Sprite::respondOnLeftButtonClick(LPARAM lParam)
{
	COORD passedPosition = getPositionFromLparam(lParam);

	bool inImageBounds = (passedPosition.X >= position_.X) &&
		(passedPosition.X <= position_.X + imageSize_.cx) &&
		(passedPosition.Y >= position_.Y) &&
		(passedPosition.Y <= position_.Y + imageSize_.cy);

	if (inImageBounds && !isClicked_)
	{
		isClicked_ = true;
	}
	else
	{
		isClicked_ = false;
	}
}

void Sprite::respondOnLeftButtonRelease()
{
	isClicked_ = false;
}

void Sprite::respondOnDragging(WPARAM wParam, LPARAM lParam)
{
	if (isClicked_)
	{
		fillBackground(DEFAULT_BACKGROUND_COLOR);

		COORD passedPosition = getPositionFromLparam(lParam);
		position_ = {
			passedPosition.X - static_cast<SHORT>(imageSize_.cx) / 2,
			passedPosition.Y - static_cast<SHORT>(imageSize_.cy) / 2
		};

		returnToWindow(&position_);
		update();
	}
}

bool Sprite::respondOnScrolling(WPARAM wParam)
{
	COORD shift;
	if (GET_KEYSTATE_WPARAM(wParam) == MK_SHIFT)
	{
		shift = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? getLeftShift() : getRightShift();
	}
	else
	{
		shift = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? getUpShift() : getDownShift();
	}
	shiftOn(shift);
	return requestImageUpdating();
}

bool Sprite::respondOnKeyPress(WPARAM wParam)
{
	if (wParam == VK_L)
	{
		return requestImageLoading();
	}
	else
	{
		switch (wParam)
		{
		case VK_LEFT:
		case VK_A:
			shiftOn(getLeftShift());
			break;
		case VK_RIGHT:
		case VK_D:
			shiftOn(getRightShift());
			break;
		case VK_DOWN:
		case VK_S:
			shiftOn(getDownShift());
			break;
		case VK_UP:
		case VK_W:
			shiftOn(getUpShift());
			break;
		case VK_Q:
			angle_ -= DEFAULT_ANGLE;
			break;
		case VK_E:
			angle_ += DEFAULT_ANGLE;
			break;
		case VK_H:
			reflected_.isHorizontal = !reflected_.isHorizontal;
			break;
		case VK_V:
			reflected_.isVertical = !reflected_.isVertical;
			break;
		}
		return requestImageUpdating();
	}
	return true;
}

void Sprite::respondOnRightButtonClick(LPARAM lParam)
{
	if (!isAnimated_)
	{
		COORD clickPosition = getPositionFromLparam(lParam);
		COORD imageCenter = {
			position_.X + static_cast<SHORT>(imageSize_.cx) / 2,
			position_.Y + static_cast<SHORT>(imageSize_.cy) / 2
		};

		SHORT xDirection = (imageCenter.X < clickPosition.X) ? 1 : -1;
		SHORT yDirection = (imageCenter.Y < clickPosition.Y) ? 1 : -1;

		SHORT clickDeltaX = abs(clickPosition.X - imageCenter.X);
		SHORT clickDeltaY = abs(clickPosition.Y - imageCenter.Y);
		SHORT clickDistance = static_cast<SHORT>(pow(pow(static_cast<double>(clickDeltaX), 2.0) + 
			pow(static_cast<double>(clickDeltaY), 2.0), 
			0.5));

		SHORT distance = static_cast<SHORT>(DEFAULT_SHIFT * 0.8);
		SHORT deltaX = static_cast<SHORT>(static_cast<double>(clickDeltaX) / clickDistance * distance) * xDirection;
		SHORT deltaY = static_cast<SHORT>(static_cast<double>(clickDeltaY) / clickDistance * distance) * yDirection;

		if ((deltaX == 0) && (deltaY == 0))
		{
			return;
		}
		animationShift_ = { deltaX, deltaY };

		isAnimated_ = true;
		SetTimer(hWnd_, TIMER_NO, 30, 0);
	}
	else
	{
		isAnimated_ = false;
		KillTimer(hWnd_, TIMER_NO);
	}
}

void Sprite::respondOnTimer()
{
	shiftOn(animationShift_);
	requestImageUpdating();
}


bool Sprite::loadImage()
{
	WCHAR fileName[MAX_PATH] = { };

	OPENFILENAME openFileName;
	openFileName.lStructSize = sizeof(OPENFILENAME);
	openFileName.hwndOwner = hWnd_;
	openFileName.hInstance = 0;
	openFileName.lpstrFilter = L"Images\0*.bmp;*.gif;*.jpeg;*.png\0\0";
	openFileName.lpstrCustomFilter = 0;
	openFileName.nFilterIndex = 1;
	openFileName.lpstrFile = fileName;
	openFileName.nMaxFile = sizeof(fileName);
	openFileName.lpstrFileTitle = 0;
	openFileName.lpstrInitialDir = 0;
	openFileName.lpstrTitle = L"Select image";
	openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	openFileName.lpstrDefExt = 0;

	bool sucessful = GetOpenFileName(&openFileName);
	if (sucessful)
	{
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		Gdiplus::Bitmap *sourceImage = Gdiplus::Bitmap::FromFile(fileName);
		HBITMAP hBitmap;
		Gdiplus::Status bitmapStatus = sourceImage->GetHBITMAP(DEFAULT_BACKGROUND_COLOR, &hBitmap);

		Gdiplus::GdiplusShutdown(gdiplusToken);

		if (bitmapStatus != Gdiplus::Ok)
		{
			return false;
		}

		SIZE windowSize = getWindowSize(hWnd_);

		bool canWindowContainImage = ((windowSize.cx >= imageSize_.cx) && (windowSize.cy >= imageSize_.cy));
		if (!canWindowContainImage)
		{
			return false;
		}

		image_ = hBitmap;
		sucessful = true;
	}
	return sucessful;
}

COORD Sprite::shiftOn(COORD shift)
{
	COORD newSpritePosition{ position_.X + shift.X, position_.Y + shift.Y };

	returnToWindow(&newSpritePosition);

	position_ = newSpritePosition;

	return newSpritePosition;
}

void Sprite::returnToWindow(COORD* coordinates)
{
	SIZE windowSize = getWindowSize(hWnd_);

	if ((*coordinates).X == -DEFAULT_SHIFT)
	{
		(*coordinates).X = DEFAULT_SHIFT / 2;
		if (isAnimated_)
		{
			animationShift_.X = -animationShift_.X;
		}
	}
	else if ((*coordinates).X < 0)
	{
		(*coordinates).X = 0;
		if (isAnimated_)
		{
			animationShift_.X = -animationShift_.X;
		}
	}
	else if ((*coordinates).X + imageSize_.cx == windowSize.cx + DEFAULT_SHIFT)
	{
		(*coordinates).X = static_cast<SHORT>(windowSize.cx - imageSize_.cx - DEFAULT_SHIFT / 2);
		if (isAnimated_)
		{
			animationShift_.X = -animationShift_.X;
		}
	}
	else if ((*coordinates).X + imageSize_.cx > windowSize.cx)
	{
		(*coordinates).X = static_cast<SHORT>(windowSize.cx - imageSize_.cx);
		if (isAnimated_)
		{
			animationShift_.X = -animationShift_.X;
		}
	}


	if ((*coordinates).Y == -DEFAULT_SHIFT)
	{
		(*coordinates).Y = DEFAULT_SHIFT / 2;
		if (isAnimated_)
		{
			animationShift_.Y = -animationShift_.Y;
		}
	}
	else if ((*coordinates).Y < 0)
	{
		(*coordinates).Y = 0;
		if (isAnimated_)
		{
			animationShift_.Y = -animationShift_.Y;
		}
	}
	else if ((*coordinates).Y + imageSize_.cy == windowSize.cy + DEFAULT_SHIFT)
	{
		(*coordinates).Y = static_cast<SHORT>(windowSize.cy - imageSize_.cy - DEFAULT_SHIFT / 2);
		if (isAnimated_)
		{
			animationShift_.Y = -animationShift_.Y;
		}
	}
	else if ((*coordinates).Y + imageSize_.cy > windowSize.cy)
	{
		(*coordinates).Y = static_cast<SHORT>(windowSize.cy - imageSize_.cy);
		if (isAnimated_)
		{
			animationShift_.Y = -animationShift_.Y;
		}
	}
}

bool Sprite::placeSprite()
{
	HDC wndDC = GetDC(hWnd_);
	HDC spriteDC = CreateCompatibleDC(wndDC);
	HGDIOBJ oldObject = SelectObject(spriteDC, image_);

	XFORM xForm;
	int prevGraphicsMode = SetGraphicsMode(wndDC, GM_ADVANCED);

	xForm = TransformationMatrix::getMovementStruct(position_);
	SetWorldTransform(wndDC, &xForm);

	COORD transformationCenter;
	transformationCenter.X = static_cast<SHORT>(-(position_.X + imageSize_.cx / 2));
	transformationCenter.Y = static_cast<SHORT>(-(position_.Y + imageSize_.cy / 2));
	xForm = TransformationMatrix::getMovementStruct(transformationCenter);
	ModifyWorldTransform(wndDC, &xForm, MWT_RIGHTMULTIPLY);

	xForm = TransformationMatrix::getRotationStruct(angle_);
	ModifyWorldTransform(wndDC, &xForm, MWT_RIGHTMULTIPLY);

	xForm = TransformationMatrix::getReflectionStruct(reflected_);
	ModifyWorldTransform(wndDC, &xForm, MWT_RIGHTMULTIPLY);

	transformationCenter.X = -transformationCenter.X;
	transformationCenter.Y = -transformationCenter.Y;
	xForm = TransformationMatrix::getMovementStruct(transformationCenter);
	ModifyWorldTransform(wndDC, &xForm, MWT_RIGHTMULTIPLY);

	bool result = BitBlt(wndDC, 0, 0, imageSize_.cx, imageSize_.cy, spriteDC, 0, 0, SRCCOPY);
	ModifyWorldTransform(wndDC, NULL, MWT_IDENTITY);
	SetGraphicsMode(wndDC, prevGraphicsMode);

	SelectObject(spriteDC, oldObject);
	DeleteDC(spriteDC);
	ReleaseDC(hWnd_, wndDC);
	return result;
}

COORD Sprite::getUpShift()
{
	return { 0, -DEFAULT_SHIFT };
}

COORD Sprite::getLeftShift()
{
	return { -DEFAULT_SHIFT, 0 };
}

COORD Sprite::getDownShift()
{
	return { 0, DEFAULT_SHIFT };
}

COORD Sprite::getRightShift()
{
	return { DEFAULT_SHIFT, 0 };
}


bool Sprite::requestImageLoading()
{
	return PostMessage(hWnd_, WM_SPRITE_LOADING, NULL, NULL);
}

bool Sprite::requestImageUpdating()
{
	return PostMessage(hWnd_, WM_SPRITE_UPDATING, NULL, NULL);
}

bool Sprite::requestDefaultImageLoading(HINSTANCE hInstance)
{
	return PostMessage(hWnd_, WM_SPRITE_DEFAULT_IMAGE, NULL, (LPARAM)hInstance);
}


SIZE Sprite::getWindowSize(HWND hWnd)
{
	RECT r;
	GetClientRect(hWnd, &r);
	SIZE result;
	result.cx = r.right - r.left;
	result.cy = r.bottom - r.top;
	return result;
}

SIZE Sprite::getImageSize(HBITMAP hBitmap)
{
	BITMAP b;
	GetObject(hBitmap, sizeof(BITMAP), &b);
	SIZE result;
	result.cx = b.bmWidth;
	result.cy = b.bmHeight;
	return result;
}

COORD Sprite::getPositionFromLparam(LPARAM lParam)
{
	return { static_cast<SHORT>(LOWORD(lParam)), static_cast<SHORT>(HIWORD(lParam)) };
}

bool Sprite::isLeftButtonClicked(WPARAM wParam)
{
	return LOWORD(wParam) == MK_LBUTTON;
}

bool Sprite::isRightButtonClicked(WPARAM wParam)
{
	return LOWORD(wParam) == MK_RBUTTON;
}

int Sprite::fillBackground(COLORREF color)
{
	RECT r;
	GetClientRect(hWnd_, &r);
	HDC wndDC = GetDC(hWnd_);
	HBRUSH hBrush = CreateSolidBrush(color);
	int result = FillRect(wndDC, &r, hBrush);
	DeleteObject(hBrush);
	ReleaseDC(hWnd_, wndDC);
	return result;
}

