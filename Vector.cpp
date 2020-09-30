#include "Vector.h"

Vector Vector::normalize() //Color should not
{
	float mag = sqrt(x * x + y * y + z * z);
	if (mag > 0)
	{
		float rev_mag = 1 / mag;
		x *= rev_mag; y *= rev_mag; z *= rev_mag;
		w = 1.0f;
	}
	return *this;
}

Vector::Vector(const Vector& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
	w = vec.w;
}

Vector crossProduct(const Vector& va, const Vector& vb)
{
	return Vector(
		va.y * vb.z - va.z * vb.y,
		va.z * vb.x - va.x * vb.z,
		va.x * vb.y - va.y * vb.x,
		1.0f
	);
}