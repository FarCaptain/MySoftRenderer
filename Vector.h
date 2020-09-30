#pragma once
#include "Config.h"
#include <cmath>

struct Vector
{
	float x, y, z, w;
	Vector() :x(0.f), y(0.f), z(0.f), w(1.0f) { };
	Vector(float _x, float _y, float _z, float _w = 1.0f)
		:x(_x), y(_y), z(_z), w(_w) { }

	Vector(const Vector& a);

	Vector normalize();

	Vector operator -() const
	{
		return Vector(-x, -y, -z, 1.0f);
	}

	Vector &operator =(const Vector& vec)
	{
		x = vec.x; y = vec.y; z = vec.z; w = vec.w;
		return *this;
	}

	Vector operator +(const Vector& vec) const
	{
		return Vector(x + vec.x, y + vec.y, z + vec.z, 1.0f);
	}

	Vector operator -(const Vector& vec) const
	{
		return Vector(x - vec.x, y - vec.y, z - vec.z, 1.0f);
	}

	Vector operator *(const float a) const
	{
		return Vector(a * x, a * y, a * z, 1.0f);
	}

	float operator *(const Vector& vec) const //dot product
	{
		return x * vec.x + y * vec.y + z * vec.z;
	}
};

Vector crossProduct(const Vector& va, const Vector& vb);