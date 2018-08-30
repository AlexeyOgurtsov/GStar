#pragma once

#include "MathUtilsMinimal.h"
#include <boost/functional/hash_fwd.hpp>
#include "Core/Stream/StreamTraits.h"

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
	* Performs normalization on this vector object.
	*/
	void Normalize(float& OutLength);

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

	/**
	* boost::serialization support.
	*/
	template<class Archive>
	void serialize(Archive& Ar, const unsigned int Version)
	{
		Ar & X;
		Ar & Y;
		Ar & Z;
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

__forceinline float SqrDistance(const Vector& A, const Vector& B)
{
	return (A - B).SqrLength();
}

__forceinline Vector NormalizedDirection(const Vector& From, const Vector& To, float& OutLength)
{
	Vector Dir = (To - From);
	Dir.Normalize(OutLength);
	return Dir;
}

__forceinline Vector NormalizedDirection(const Vector& From, const Vector& To)
{
	float Length;
	return NormalizedDirection(From, To, /*Out*/Length);
}

__forceinline float Distance(const Vector& A, const Vector& B)
{
	return M::Sqrt(SqrDistance(A, B));
}

inline bool operator==(const Vector& A, const Vector& B)
{
	return A.X == B.X && A.Y == B.Y && A.Z == B.Z;
}

inline bool operator!=(const Vector& A, const Vector& B)
{
	return !(A == B);
}

bool operator<(const Vector& A, const Vector& B);
inline bool operator<=(const Vector& A, const Vector& B)
{
	return A < B || A == B;
}
inline bool operator>(const Vector& A, const Vector& B)
{
	return ! (A <= B);
}
inline bool operator>=(const Vector& A, const Vector& B)
{
	return ! (A < B);
}

constexpr float VEC_EQUAL_EPS = M::EPS;

inline bool Equals(const Vector& A, const Vector& B, float Tolerance = VEC_EQUAL_EPS)
{
	return M::Abs(A.X - B.X) <= Tolerance 
		&& M::Abs(A.Y - B.Y) <= Tolerance 
		&& M::Abs(A.Z - B.Z) <= Tolerance;
}

inline bool NotEquals(const Vector& A, const Vector& B, float Tolerance = VEC_EQUAL_EPS)
{
	return ! Equals(A, B, Tolerance);
}

template<class Strm, typename = typename std::enable_if<IsOutputStream<Strm>::Value>::type>
Strm& operator<<(Strm& S, const Vector& V)
{
	return S << "{" << V.X << ";" << V.Y << ";" << V.Z << "}";
}

template<class Strm, typename = typename std::enable_if<IsInputStream<Strm>::Value>::type>
Strm& operator>>(Strm& S, Vector& V)
{
	return S >> V.X >> V.Y >> V.Z;
}

/**
* Prints C-str representation of the vector into the given buffer.
* If buffer is not enough, the rest of the characters are discarded.
*
* @returns: count of characters printed.
*/
int PrintVector(char* pBuf, int BufSize, const Vector& V);

namespace std
{
	template<> struct hash<Vector>
	{
		size_t operator() (const Vector& V) const
		{
			size_t H = 0;
			boost::hash_combine(H, V.X);
			boost::hash_combine(H, V.Y);
			boost::hash_combine(H, V.Z);
			return H;
		}
	};
}

/**
* TODO:
*
* 1. Stream operators.
* 2. Compare operators.
*
* Optimization:
* 1. SSE etc.
*/