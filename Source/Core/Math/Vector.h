#pragma once

/**
* 4D vector.
*/
struct Vector
{
	float X;
	float Y;
	float Z;
	float W;

	/**
	* Default ctor: WARNING!!! Creates uninitialized vector object.
	*/
	Vector() = default;

	/**
	* Initializes vector with the given values.
	*/ 
	Vector(float InX, float InY, float InZ, float InW = 1.0F) :
		X(InX)
	,	Y(InY)
	,	Z(InZ)
	,	W(InW) {}

	Vector& operator+=(const Vector& B);
	Vector& operator-=(const Vector& B);

	Vector& operator*=(float Value);
	Vector& operator/=(float Value); 

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
	float SqrLength() const;
};
Vector operator-(const Vector& V);
Vector operator+(const Vector& A, const Vector& B);
Vector operator-(const Vector& A, const Vector& B);
Vector operator*(const Vector& V, float Value);
Vector operator/(const Vector& V, float Value);


/**
* Cross operation on the X,Y,Z components of the vector.
*/
Vector Cross(const Vector& A, const Vector& B);

/**
* Dot product operation.
*/
float Dot(const Vector& A, const Vector& B);

/**
* TODO:
*
* Optimization:
* 1. SSE etc.
*/