#pragma once
#include "Vector.h"
#include <iostream>

#ifndef Matrix_H
#define Matrix_H

using namespace std;

struct Matrix
{
	float mx[4][4];
	friend ostream& operator<<(ostream& out, Matrix& mat);
	Matrix();
	void identity();
};

Matrix multiply(const Matrix& mat0, const Matrix& mat1);

Vector multiply(const Vector& vec, const Matrix& mat);

Matrix rotate(const Vector& ax, float angle);

#endif // !Matrix_H