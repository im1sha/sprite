#pragma once

#include "TransformationMatrix.h"

XFORM TransformationMatrix::getRotationStruct(short degreeAngle)
{
	XFORM xForm;
	FLOAT angle = (FLOAT)(M_PI * degreeAngle / 180);
	FLOAT angleSin = sin(angle);
	FLOAT angleCos = cos(angle);
	xForm.eM11 = angleCos;
	xForm.eM12 = angleSin;
	xForm.eM21 = -angleSin;
	xForm.eM22 = angleCos;
	xForm.eDx = 0;
	xForm.eDy = 0;
	return xForm;
}

XFORM TransformationMatrix::getReflectionStruct(TransformationMatrix::Reflection reflection)
{
	XFORM xForm;
	xForm.eM11 = static_cast<float>(reflection.isVertical ? -1 : 1);
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = static_cast<float>(reflection.isHorizontal ? -1 : 1);
	xForm.eDx = 0;
	xForm.eDy = 0;
	return xForm;
}

XFORM TransformationMatrix::getMovementStruct(COORD position)
{
	XFORM xForm;
	xForm.eM11 = 1;
	xForm.eM12 = 0;
	xForm.eM21 = 0;
	xForm.eM22 = 1;
	xForm.eDx = position.X;
	xForm.eDy = position.Y;
	return xForm;
}


