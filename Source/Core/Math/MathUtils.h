#pragma once

#include "../CoreSysMinimal.h"

class Vector;

namespace M
{
	constexpr float PI = 3.1415926535897932F;
	constexpr float E = 2.71828182845904523536f;

	constexpr float HALF_PI = PI / 2.0F;
	constexpr float INV_PI = 1.F / PI;

	constexpr float EPS = 1.e-8f;
	constexpr float KINDA_EPS = 1.e-4f;

	template<class T>
	const T& Min(const T& A, const T& B)
	{
		if (A <= B) 
		{
			return A; 
		}
		else
		{
			return B;
		}
	}

	template<class T>
	const T& Max(const T& A, const T& B)
	{
		if (A <= B)
		{
			return B;
		}
		else
		{
			return A;
		}
	}
	
	float Abs(float Value);

	float Cos(float Rads);
	float Sin(float Rads);

	float CosDegs(float Degs);
	float SinDegs(float Degs);

	float Atan2(float X, float Y);

	float Sqrt(float Value);
}