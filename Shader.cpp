#include "Shader.h"

void Shader::setLight(const Vector& L, const Color& C, float _shine)
{
	Light = L;
	light_color = C;
	shine = _shine;
}

Vector Shader::wvp(const Vector& vec) const
{
	Vector ret; //
	ret = multiply(vec, world);
	ret = multiply(ret, view);
	ret = multiply(ret, projection);
	return ret;
}

Vertex Shader::wvp(const Vertex& vtx) const
{
	Vertex ret; //
	ret.pos = multiply(vtx.pos, world);
	ret.normal = multiply(vtx.normal, world);
	ret.worldpos = ret.pos;
	ret.pos = multiply(ret.pos, view);
	ret.pos = multiply(ret.pos, projection);

	ret.color = vtx.color;
	return ret;
}

void Shader::setWord(const Matrix& mat)
{
	world = mat;
}

void Shader::setView(const Vector& pos, const Vector& gaze, const Vector& up)
{
	Camera = pos;
	Matrix& ret = view;
	Vector vec_x = crossProduct(up, gaze);
	ret.mx[0][0] = vec_x.x; ret.mx[0][1] = up.x; ret.mx[0][2] = gaze.x; ret.mx[0][3] = 0.0f;
	ret.mx[1][0] = vec_x.y; ret.mx[1][1] = up.y; ret.mx[1][2] = gaze.y; ret.mx[1][3] = 0.0f;
	ret.mx[2][0] = vec_x.z; ret.mx[2][1] = up.z; ret.mx[2][2] = gaze.z; ret.mx[2][3] = 0.0f;
	ret.mx[3][0] = -pos * vec_x; ret.mx[3][1] = -pos * up; ret.mx[3][2] = -pos * gaze; ret.mx[3][3] = 1.0f;
}

void Shader::setProjection(const float fovy, const float aspect, const float n, const float f)
{
	Matrix& ret = projection;
	float h = n * tan(fovy * 0.5f);
	float w = h * aspect;
	ret.mx[0][0] = n / w; ret.mx[0][1] = 0.0f; ret.mx[0][2] = 0.0f; ret.mx[0][3] = 0.0f;
	ret.mx[1][0] = 0.0f; ret.mx[1][1] = n / h; ret.mx[1][2] = 0.0f; ret.mx[1][3] = 0.0f;
	ret.mx[2][0] = 0.0f; ret.mx[2][1] = 0.0f; ret.mx[2][2] = f / (f - n); ret.mx[2][3] = 1.0f;
	ret.mx[3][0] = 0.0f; ret.mx[3][1] = 0.0f; ret.mx[3][2] = n * f / (n - f); ret.mx[3][3] = 0.0f;
}

Vertex Shader::vertexShader(const Vertex& vtx) const
{
	Vertex ret; //
	ret.pos = multiply(vtx.pos, world);
	ret.normal = multiply(vtx.normal, world);
	ret.worldpos = ret.pos;
	ret.pos = multiply(ret.pos, view);
	ret.pos = multiply(ret.pos, projection);

	ret.color = vtx.color;
	return ret;
}

Color Shader::pixelShader(const float& Ka, const Color& Kd, const Color& Ks) const
{
	Color shade, ambient, diffuse, specular;
	ambient = pixel.color * 0.3f;
	diffuse = Kd * pixel.color * saturate((-Light) * pixel.normal);
	Vector View = Camera - pixel.world_pos;
	Vector H = (View + (-Light)).normalize();
	specular = Ks * pow(saturate(H * pixel.normal), shine);
	return ambient + diffuse + specular;
}

void Shader::setPixel(const Vector& _n, const Vector& _w, const Color& _c)
{
	pixel.normal = _n; pixel.world_pos = _w; pixel.color = _c;
}