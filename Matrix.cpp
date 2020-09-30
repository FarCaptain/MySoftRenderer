#include"Matrix.h"

Matrix::Matrix()
{
	identity();
}

ostream& operator<<(ostream& out, Matrix& mat)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			cout << mat.mx[i][j] << " ";
		}
		cout << endl;
	}
	return out;
}

void Matrix::identity()
{
	mx[0][0] = 1.f; mx[0][1] = 0.f; mx[0][2] = 0.f; mx[0][3] = 0.f;
	mx[1][0] = 0.f; mx[1][1] = 1.f; mx[1][2] = 0.f; mx[1][3] = 0.f;
	mx[2][0] = 0.f; mx[2][1] = 0.f; mx[2][2] = 1.f; mx[2][3] = 0.f;
	mx[3][0] = 0.f; mx[3][1] = 0.f; mx[3][2] = 0.f; mx[3][3] = 1.f;
}

Matrix multiply(const Matrix& mat0, const Matrix& mat1)
{
	Matrix ret;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			ret.mx[i][j] = 0;
			for (int k = 0; k < 4; ++k)
				ret.mx[i][j] += mat0.mx[i][k] * mat1.mx[k][j];
		}
	}
	return ret;
}

Vector multiply(const Vector& vec, const Matrix& mat)
{
	Vector res;
	res.x = vec.x * mat.mx[0][0] + vec.y * mat.mx[1][0] + vec.z * mat.mx[2][0] + vec.w * mat.mx[3][0];
	res.y = vec.x * mat.mx[0][1] + vec.y * mat.mx[1][1] + vec.z * mat.mx[2][1] + vec.w * mat.mx[3][1];
	res.z = vec.x * mat.mx[0][2] + vec.y * mat.mx[1][2] + vec.z * mat.mx[2][2] + vec.w * mat.mx[3][2];
	res.w = vec.x * mat.mx[0][3] + vec.y * mat.mx[1][3] + vec.z * mat.mx[2][3] + vec.w * mat.mx[3][3];
	return res;
}

Matrix rotate(const Vector& ax, float angle)
{
	Matrix ret;
	float sin_val = sin(angle);
	float cos_val = cos(angle);
	float cos_rev = 1.0f - cos_val;

	//ax.normalize();
	ret.mx[0][0] = cos_rev * ax.x * ax.x + cos_val;
	ret.mx[0][1] = cos_rev * ax.x * ax.y + sin_val * ax.z;
	ret.mx[0][2] = cos_rev * ax.x * ax.z - sin_val * ax.y;
	ret.mx[0][3] = 0.f;

	ret.mx[1][0] = cos_rev * ax.x * ax.y - sin_val * ax.z;
	ret.mx[1][1] = cos_rev * ax.y * ax.y + cos_val;
	ret.mx[1][2] = cos_rev * ax.y * ax.z + sin_val * ax.x;
	ret.mx[1][3] = 0.f;

	ret.mx[2][0] = cos_rev * ax.x * ax.z + sin_val * ax.y;
	ret.mx[2][1] = cos_rev * ax.y * ax.z - sin_val * ax.x;
	ret.mx[2][2] = cos_rev * ax.z * ax.z + cos_val;
	ret.mx[2][3] = 0.f;

	ret.mx[3][0] = 0.f;
	ret.mx[3][1] = 0.f;
	ret.mx[3][2] = 0.f;
	ret.mx[3][3] = 1.f;

	return ret;
}