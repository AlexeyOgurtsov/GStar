#include "../Vector.h"
#include "../MathUtils.h"
#include <cstdio>

/**
* Performs normalization on this vector object.
*/
void Vector::Normalize(float& OutLength)
{
	OutLength = Length();
	*this /= OutLength;
}

void Vector::Normalize()
{
	float Len;
	Normalize(/*Out*/Len);
}

float Vector::Length() const
{
	return M::Sqrt(SqrLength());
}

bool operator<(const Vector& A, const Vector& B)
{
	if (A.X < B.X)
	{
		return true;
	}

	if (A.X > B.X)
	{
		return false;
	}

	if (A.Y < B.Y)
	{
		return true;
	}

	if (A.Y > B.Y)
	{
		return false;
	}

	if (A.Z < B.Z)
	{
		return true;
	}

	// Extra check
	//if (A.Z > B.Z)
	//{
	//	return false;
	//}

	return false;
}

int PrintVector(char* pBuf, int BufSize, const Vector& V)
{
	BOOST_ASSERT(pBuf);
	return snprintf(pBuf, BufSize, "{%f;%f;%f}", V.X, V.Y, V.Z);
}