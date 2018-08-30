#include "../MathUtils.h"
#include "../Vector.h"
#include <cmath>

/**
* Warning!!! Put all math static variable definitions in this file to avoid initialization order errors!
*/
const Vector Vector::OrtX{ 1.F,0,0 };
const Vector Vector::OrtY{ 0,1.F,0 };
const Vector Vector::OrtZ{ 0,0,1.F };

namespace M
{
	float Abs(float Value)
	{
		return Value < 0 ? -Value : Value;
	}

	float Cos(float Rads)
	{
		return static_cast<float>(cos(Rads));
	}

	float Sin(float Rads)
	{
		return static_cast<float>(sin(Rads));
	}

	float CosDegs(float Degs)
	{
		return cosf(Degs * DEGS_TO_RADIANS);
	}

	float SinDegs(float Degs)
	{
		return sinf(Degs * DEGS_TO_RADIANS);
	}

	void SinCos(float Rads, float& OutSin, float& OutCos)
	{
		OutSin = Sin(Rads);
		OutCos = Cos(Rads);
	}

	float Atan2(float Y, float X)
	{
		return atan2f(Y, X);
	}

	void SinCosDegs(float Degs, float& OutSin, float& OutCos)
	{
		SinCos(Degs * DEGS_TO_RADIANS, OutSin, OutCos);
	}

	float Sqrt(float Value)
	{
		return sqrtf(Value);
	}
} // M