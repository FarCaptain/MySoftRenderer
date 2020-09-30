#pragma once
#include "Config.h"
#include <cmath>

static float saturate(float x)
{
	return fmin(1.0f, fmax(0.0f, x));
}

struct Color {
	float r, g, b, a;
	Color() :r(0.f), g(0.f), b(0.f), a(0.f) {}
	Color(float _r, float _g, float _b, float _a = 1.0f) :r(_r), g(_g), b(_b), a(_a) {}

	Color operator +(const Color& x) const {
		return Color(r + x.r, g + x.g, b + x.b, a + x.a);
	}

	Color operator *(const float x) const {
		return Color(x * r, x * g, x * b, x * a);
	}

	Color operator *(const Color& w) const {
		return Color(r * w.r, g * w.g, b * w.b, a * w.a);
	}

	operator uint32()
	{
		uint32 R = (uint32)round(saturate(r) * 255);
		uint32 G = (uint32)round(saturate(g) * 255);
		uint32 B = (uint32)round(saturate(b) * 255);
		return R << 16 | G << 8 | B; //what about A?
	}
};