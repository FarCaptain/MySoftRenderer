#include"Renderer.h"

Renderer::Renderer(uint32 _w, uint32 _h, uint32* _fb) : width(_w), height(_h), fb(_fb)
{
	framebuffer = new uint32 * [height];
	for (uint32 i = 0; i < height; ++i) {
		framebuffer[i] = fb + i * width;
	}
}

Renderer::~Renderer()
{
	delete[] framebuffer;
}

void Renderer::clear()
{
	for (uint32 i = 0; i < height; i++) {
		for (uint32 j = 0; j < width; ++j) {
			framebuffer[i][j] = 0x0;
		}
	}
}

Vector Renderer::homogenize(const Vector& vec)
{
	float rev_w = 1.0f / vec.w;
	return Vector(
		(1.f + vec.x * rev_w) * 0.5f * width,
		(1.f - vec.y * rev_w) * 0.5f * height,
		vec.z * rev_w,
		1.0f
	);
}

void Renderer::drawPoint(const int x, const int y, const uint32 c)
{
	framebuffer[y][x] = c;
}

void Renderer::drawPoint(const float x, const float y, const uint32 c)
{
	int _y = Round(y), _x = Round(x);
	framebuffer[_y][_x] = c;
}

bool Renderer::checkCVV(const Vector& vec)
{
	float w = vec.w;
	return (vec.x < w && vec.x > -w && vec.y < w&& vec.y > -w && vec.z > 0 && vec.z < w);
}

void Renderer::drawLine3D(const Vector& v1, const Vector& v2, const Color& c1, const Color& c2)
{
	Vector st = shader.wvp(v1);
	Vector ed = shader.wvp(v2);

	//check CVV
	if (!checkCVV(st))return;
	if (!checkCVV(ed))return;

	float w1 = 1.f / st.w;
	float w2 = 1.f / ed.w;

	st = homogenize(st);
	ed = homogenize(ed);

	int x0 = Round(st.x);
	int y0 = Round(st.y);
	int x1 = Round(ed.x);
	int y1 = Round(ed.y);

	int dx = x1 - x0;
	int dy = y1 - y0;
	bool steep = false;
	if (abs(dx) < abs(dy))
	{
		swap(x1, y1);
		swap(x0, y0);
		swap(dx, dy);
		steep = true;
	}

	int delta = 0;
	int y = y0;
	int ux = (dx > 0) ? 1 : -1;
	int uy = (dy > 0) ? 1 : -1;
	uint32 color;
	dx = abs(dx); dy = abs(dy);
	float dt = 1.0f / dx;
	float t = 1.f;
	for (int x = x0; x != (x1 + ux); x += ux)
	{
		float zprime = 1.f / (t * w1 + (1.f - t) * w2);
		color = (uint32)((c1 * t * w1 + c2 * (1.f - t) * w2) * zprime); //lerp
		t -= dt;

		if (steep)
			drawPoint(y, x, color);
		else
			drawPoint(x, y, color);

		delta += dy;
		if (delta + delta >= dx)
		{
			delta -= dx;
			y += uy;
		}
	}
}

void Renderer::drawLine2D(int x0, int y0, int x1, int y1, const Color &c1, const Color &c2)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	bool steep = false;
	if (abs(dx) < abs(dy))
	{
		swap(x1, y1);
		swap(x0, y0);
		swap(dx, dy);
		steep = true;
	}

	int delta = 0;
	int y = y0;
	int ux = (dx > 0) ? 1 : -1;
	int uy = (dy > 0) ? 1 : -1;
	uint32 color;
	dx = abs(dx); dy = abs(dy);
	float dt = 1.0f / dx;
	float t = 1.f;
	for (int x = x0; x != (x1 + ux); x += ux)
	{
		color = (uint32)(c1 * t + c2 * (1 - t)); //lerp
		t -= dt;

		if (steep)
			drawPoint(y, x, color);
		else
			drawPoint(x, y, color);

		delta += dy;
		if (delta + delta >= dx)
		{
			delta -= dx;
			y += uy;
		}
	}
}

void Renderer::drawTriangle(const Vertex& vt1, const Vertex& vt2, const Vertex& vt3)
{
	Vertex v1 = shader.vertexShader(vt1);
	Vertex v2 = shader.vertexShader(vt2);
	Vertex v3 = shader.vertexShader(vt3);

	Vector p0 = v1.pos;
	Vector p1 = v2.pos;
	Vector p2 = v3.pos;

	if (!checkCVV(p0)) return;
	if (!checkCVV(p1)) return;
	if (!checkCVV(p2)) return;

	float w0 = 1.f / p0.w;
	float w1 = 1.f / p1.w;
	float w2 = 1.f / p2.w;

	p0 = homogenize(p0);
	p1 = homogenize(p1);
	p2 = homogenize(p2);

	int x0 = Round(Min(p0.x, p1.x, p2.x));
	int x1 = Round(Max(p0.x, p1.x, p2.x));
	int y0 = Round(Min(p0.y, p1.y, p2.y));
	int y1 = Round(Max(p0.y, p1.y, p2.y));

	float f01 = (p0.y - p1.y) * p2.x + (p1.x - p0.x) * p2.y + p0.x * p1.y - p1.x * p0.y;
	float f20 = (p2.y - p0.y) * p1.x + (p0.x - p2.x) * p1.y + p2.x * p0.y - p0.x * p2.y;

	for (int x = x0; x <= x1; ++x) {
		for (int y = y0; y <= y1; ++y) {
			float gamma = ((p0.y - p1.y) * x + (p1.x - p0.x) * y + p0.x * p1.y - p1.x * p0.y) / f01;
			float beta = ((p2.y - p0.y) * x + (p0.x - p2.x) * y + p2.x * p0.y - p0.x * p2.y) / f20;
			float alpha = 1.f - beta - gamma;

			if (alpha >= 0.f && beta >= 0.f && gamma >= 0.f) {
				float zprime = 1.f / (alpha * w0 + beta * w1 + gamma * w2);
				Color C = (v1.color * alpha * w0 + v2.color * beta * w1 + v3.color * gamma * w2) * zprime;
				Vector N = (v1.normal * alpha * w0 + v2.normal * beta * w1 + v3.normal * gamma * w2) * zprime;//插值得到原来的normal
				Vector W = (v1.worldpos * alpha * w0 + v2.worldpos * beta * w1 + v3.worldpos * gamma * w2) * zprime;

				shader.setPixel(N, W, C);

				float Ka = 0.3f;
				Color Kd = shader.light_color;
				Color Ks = shader.light_color;
				C = shader.pixelShader(Ka, Kd, Ks);
				drawPoint(x, y, (uint32)C);
			}
		}
	}
}