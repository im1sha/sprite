#pragma once

#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include <cmath>
#include "GraphicDefines.h"
#include "TransformationMatrix.h"
#include "resource.h"


class Sprite {
public:
	Sprite(HWND hWnd, HINSTANCE hInstanse);

	~Sprite();

	bool load();

	bool respondOnKeyPress(WPARAM wParam);

	void respondOnTimer();

	bool update();

	bool loadDefault(LPARAM lParam);

	void respondOnLeftButtonClick(LPARAM lParam);

	void respondOnRightButtonClick(LPARAM lParam);

	void respondOnLeftButtonRelease();

	void respondOnDragging(WPARAM wParam, LPARAM lParam);

	bool respondOnScrolling(WPARAM wParam);

private:
	const SHORT DEFAULT_ANGLE = 18;
	const SHORT DEFAULT_SHIFT = 20;

	HWND hWnd_ = 0;

	HBITMAP image_ = 0;
	COORD position_ = { 0, 0 };
	short angle_ = 0;
	SIZE imageSize_ = { 0, 0 };

	bool isClicked_ = false;

	UINT_PTR timerNo_ = 1;
	bool isAnimated_ = false;
	COORD animationShift_ = { 0, 0 };

	TransformationMatrix::Reflection reflected_ = { false, false };

	void initializeSprite();

	bool loadImage();

	COORD shiftOn(COORD shift);

	void returnToWindow(COORD* coordinates);

	bool placeSprite();


	COORD getUpShift();

	COORD getLeftShift();

	COORD getDownShift();

	COORD getRightShift();


	bool requestImageLoading();

	bool requestImageUpdating();

	bool requestDefaultImageLoading(HINSTANCE hInstance);


	SIZE getWindowSize(HWND hWnd);

	SIZE getImageSize(HBITMAP hBitmap);

	COORD getPositionFromLparam(LPARAM lParam);

	bool isLeftButtonClicked(WPARAM wParam);

	bool isRightButtonClicked(WPARAM wParam);

	int fillBackground(COLORREF color);
};