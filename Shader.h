#pragma once
#include "Config.h"
#include "Vertex.h"
#include"Matrix.h"
#include"Color.h"

struct Pixel
{
public:
	Vector normal;
	Vector world_pos;
	Color color;
};

class Shader
{
public:
	Vector Light;
	Vector Camera;
	Pixel pixel;
	Color light_color;
	Color specolor;
	float shine = 1.0f;

	Matrix world;
	Matrix view;
	Matrix projection;

	void setLight(const Vector& L, const Color& C, float _shine);

	void setWord(const Matrix& mat);
	void setView(const Vector& pos, const Vector& gaze, const Vector& up);
	void setProjection(float fovy, float aspect, float n, float f);
	Vector wvp(const Vector& vec) const;
	Vertex wvp(const Vertex& vtx) const;

	Vertex vertexShader(const Vertex& vtx) const;
	Color pixelShader(const float& Ka, const Color& Kd, const Color& Ks) const;
	void setPixel(const Vector& _n, const Vector& _w, const Color& _c);
};