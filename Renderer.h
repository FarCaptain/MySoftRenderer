#pragma once
#include <stdlib.h>
#include <algorithm>
#include"Config.h"
#include"Vertex.h"
#include"Matrix.h"
#include"Color.h"
#include"Shader.h"
using namespace std;

class Renderer
{
private:
	const uint32 width, height;
	uint32** framebuffer = NULL;
	uint32* fb = NULL;

public:
	Shader shader;

	Renderer(uint32 _w, uint32 _h, uint32* _fb);
	~Renderer();

	void clear();

	bool checkCVV(const Vector& vec);
	Vector homogenize(const Vector& vec);

	void drawPoint(const int x, const int y, const uint32 c);
	void drawPoint(const float x, const float y, const uint32 c);
	void drawLine3D(const Vector& st, const Vector& ed, const Color& c1, const Color& c2);
	void drawLine2D(int x0, int y0, int x1, int y1, const Color& c0, const Color& c1);
	void drawTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);
};

static float Min(const float x, const float y, const float z)
{
	return min(min(x, y), z);
}

static float Max(const float x, const float y, const float z)
{
	return max(max(x, y), z);
}

static int Round(const float x)
{
	return int(x + 0.5f);
}