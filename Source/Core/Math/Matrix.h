#pragma once

#include "Vector.h"

struct Matrix
{
	float M[4][4];

	/**
	* Default ctor: creates UNINITIALIZED matrix;
	*/
	Matrix() = default;

	/**
	* Constructs matrix from vectors.
	*/
	Matrix(const Vector& InX, const Vector& InY, const Vector& InZ, const Vector& InW)
	{
		M[0][0] = InX.X; M[0][1] = InX.Y; M[0][2] = InX.Z;
		M[1][0] = InY.X; M[1][1] = InY.Y; M[1][2] = InY.Z;
		M[2][0] = InZ.X; M[2][1] = InZ.Y; M[2][2] = InZ.Z;
		M[3][0] = InW.X; M[3][1] = InW.Y; M[3][2] = InW.Z;
		M[0][3] = M[1][3] = M[2][3] = 0;
		M[3][3] = 1.F;
	}
};

/**
* TODO Alignment:
* 1. Make always aligned 16 bytes
*
* TODO Constants:
* 1. Add identity matrix constant
*
* TODO Std:
* 1. serialiation hash
* 2. hash
* 3. comparison?
* 4. stream output
*/