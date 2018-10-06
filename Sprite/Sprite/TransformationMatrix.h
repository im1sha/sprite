#pragma once

#define _USE_MATH_DEFINES 
#include <cmath>  

#include "GraphicDefines.h"

class TransformationMatrix
{
public:
	struct Reflection
	{
		bool isHorizontal;
		bool isVertical;
	};

	static XFORM getRotationStruct(short degreeAngle);

	static XFORM getReflectionStruct(TransformationMatrix::Reflection reflection);

	static XFORM getMovementStruct(COORD position);
};