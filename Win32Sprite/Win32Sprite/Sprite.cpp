#ifndef UNICODE
#define UNICODE
#endif 

#include "Sprite.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

void Sprite::createMoveParameters()
{	
	radius_ = defaultRadius_;

	srand(static_cast<unsigned int>(time(0)));
	double initValue = ((rand() % circleInDegrees_) - semicircleInDegrees_) * pi_ / semicircleInDegrees_;

	deltaX_ = static_cast<int>(defaultSpeed_ * cos(initValue));
	deltaY_ = static_cast<int>(defaultSpeed_ * sin(initValue));
}

void Sprite::invalidate(HWND &hwnd)
{
	InvalidateRect(hwnd, &spriteRect_, true);

	x_ += deltaX_; 
	y_ += deltaY_;

	RECT clientRect;
	GetClientRect(hwnd, &clientRect);
	int width = clientRect.right;
	int height = clientRect.bottom;

	if ((x_ >= width - radius_) || (x_ < radius_)) {
		deltaX_ = -deltaX_;
	}
	if ((y_ > height - radius_) || (y_ < radius_)) {
		deltaY_ = -deltaY_;
	}

	spriteRect_.left = x_ - radius_ - 1;
	spriteRect_.top = y_ - radius_ - 1;
	spriteRect_.right = x_ + radius_ + 1;
	spriteRect_.bottom = y_ + radius_ + 1;

	InvalidateRect(hwnd, &spriteRect_, false); 
}

void Sprite::create(HWND hwnd)
{
	PAINTSTRUCT paintStruct;
	HDC hdc;
	HBRUSH brush; 
	HBRUSH oldBrush;

	hdc = BeginPaint(hwnd, &paintStruct);

	brush = CreateSolidBrush(RGB(185, 244, 66));
	oldBrush = (HBRUSH)SelectObject(hdc, brush);
	Ellipse(hdc, x_ - radius_, y_ - radius_, x_ + radius_, y_ + radius_); 

	SelectObject(hdc, oldBrush);
	DeleteObject(brush); 

	EndPaint(hwnd, &paintStruct);
}


