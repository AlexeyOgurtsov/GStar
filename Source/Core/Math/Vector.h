#pragma once

#include "../CoreSysMinimal.h"

/**
* 3D vector.
*/
struct Vector
{
	float X;
	float Y;
	float Z;

	static const Vector OrtX;
	static const Vector OrtY;
	static const Vector OrtZ;
 
	/**
	* Default ctor: WARNING!!! Creates uninitialized vector object.
	*/
	Vector() = default;

	/**
	* Initializes vector with the given values.
	*/ 
	__forceinline Vector(float InX, float InY, float InZ) :
		X(InX)
	,	Y(InY)
	,	Z(InZ) {}

	__forceinline Vector& operator+=(const Vector& B)
	{
		X += B.X; Y += B.Y; Z += B.Z;
		return *this; 
	}

	__forceinline Vector& operator-=(const Vector& B)
	{
		X -= B.X; Y -= B.Y; Z -= B.Z;
		return *this;
	}

	/**
	* Piecewise multiplication.
	*/
	__forceinline Vector& operator*=(float Value)
	{
		X *= Value; Y *= Value; Z *= Value;
		return *this;
	}

	__forceinline Vector& operator/=(float Value)
	{
		X /= Value; Y /= Value; Z /= Value;
		return *this;
	}

	/**
	* Performs normalization on this vector object.
	*/
	void Normalize();

	/**
	* Returns length.
	*/
	float Length() const;

	/**
	* Returns square length.
	*/
	__forceinline float SqrLength() const
	{
		return X * X + Y * Y + Z * Z;
	}

	__forceinline float& operator[](int32_t Index)
	{
		switch(Index)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		default:
			BOOST_ASSERT_MSG(false, "Math: Vector: operator[]: unknown index");
		}
	}

	__forceinline float operator[](int32_t Index) const
	{
		switch(Index)
		{
		case 0: return X;
		case 1: return Y;
		case 2: return Z;
		default:
			BOOST_ASSERT_MSG(false, "Math: Vector: operator[] const: unknown index");
		}
	}
};
__forceinline Vector operator-(const Vector& V)
{
	return Vector{-V.X, -V.Y, -V.Z};
}
__forceinline Vector operator+(const Vector& A, const Vector& B)
{
	return Vector{A.X + B.X, A.Y + B.Y, A.Z + B.Z};
}
__forceinline Vector operator-(const Vector& A, const Vector& B)
{
	return Vector{A.X - B.X, A.Y - B.Y, A.Z - B.Z};
}

/**
* Piecewise multiplication.
*/
__forceinline Vector operator*(const Vector& A, const Vector& B)
{
	return Vector{A.X * B.X, A.Y * B.Y, A.Z * B.Z};
}

__forceinline Vector operator*(const Vector& V, float Value)
{
	return Vector{V.X * Value, V.Y * Value, V.Z * Value};
}
__forceinline Vector operator/(const Vector& V, float Value)
{
	return Vector{V.X / Value, V.Y / Value, V.Z / Value};
}

/**
* Cross operation on the X,Y,Z components of the vector.
*/
__forceinline Vector Cross(const Vector& A, const Vector& B)
{
	return Vector
	{
		A.Y*B.Z-A.Z*B.Y,
		A.Z*B.X-A.X*B.Z,
		A.X*B.Y-A.Y*B.X 
	};
}

/**
* Dot product operation.
*/
__forceinline float Dot(const Vector& A, const Vector& B)
{
	return A.X * B.X + A.Y + B.Y + A.Z * B.Z;
}

/**
* TODO:
*
* 1. Serialization support.
* 2. Hash functions.
* 3. Stream operators.
*
* Optimization:
* 1. SSE etc.
*/