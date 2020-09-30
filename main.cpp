#include <Windows.h>
#include <WinUser.h>
#include <windef.h>
#include <WinBase.h>
#include <time.h>
#include <assert.h>
#include <map>
#include <vector>
#include <math.h>

#include "Screen.h"
#include "Config.h"
#include "Renderer.h"
#include "Matrix.h"
#include <fcntl.h>
#include <io.h>
#include <tchar.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PI 3.14159265f
Screen* screen = NULL;
Renderer* renderer = NULL;

#define VK_J 0x4A
#define VK_K 0x4B

static float r = 0.f;
Vector axis;
void init()
{
	//init
	Vector pos(0.f, 15.f, 0.f, 1.f);
	Vector gaze(0.f, -1.f, 0.f, 1.f);
	Vector up(0.f, 0.f, 1.f, 1.f);
	float aspect = (float)WINDOW_WIDTH / WINDOW_HEIGHT;
	float fovy = PI * 0.5f;
	float n = 1.f, f = 500.f;
	r = 0.f;
	axis = Vector(0, 0, 1, 1);
	axis.normalize();
	Matrix mat = rotate(axis, r);
	renderer->shader.setWord(mat);
	gaze.normalize(); up.normalize();
	renderer->shader.setView(pos, gaze, up);
	renderer->shader.setProjection(fovy, aspect, n, f);

	Vector L = Vector(0.f, -1.f, 0.f, 1.f).normalize();
	Color C = Color(1.f, 1.f, 1.f, 1.f);
	renderer->shader.setLight(L, C, 20);
}

void render()
{
	//renderer->drawLine2D(150, 200, 100, 100, Color(1, 0, 0, 1), Color(0, 1, 1, 1));
	//renderer->drawLine3D(Vector(-10, 0, 0), Vector(10, 0, 0), Color(1, 0, 0, 1), Color(0, 1, 1, 1));
	Vertex tv1, tv2, tv3;
	tv1.pos = Vector(-5.f, 0.f, 0.f);
	tv2.pos = Vector(5.f, 0.f, 0.f);
	tv3.pos = Vector(0.f, 0.f, 10.f);

	tv1.color = Color(1.f, 0.f, 0.f);
	tv2.color = Color(0.f, 1.f, 0.f);
	tv3.color = Color(0.f, 0.f, 1.f);

	tv1.normal = Vector(0.f, 1.f, 0.f);
	tv2.normal = Vector(0.f, 1.f, 0.f);
	tv3.normal = Vector(0.f, 1.f, 0.f);

	renderer->drawTriangle(tv1, tv2, tv3);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	if (AllocConsole()) {
		freopen("CONOUT$", "w", stdout);
	}
	srand((unsigned)time(NULL));

	screen = new Screen();
	int ret = screen->init(WINDOW_WIDTH, WINDOW_HEIGHT, _T("SoftRendering"));
	if (ret < 0) {
		printf("screen init failed(%d)!\n", ret);
		exit(ret);
	}
	uint32* wfb = (uint32*)(screen->getFrameBuffer());
	renderer = new Renderer(WINDOW_WIDTH, WINDOW_HEIGHT, wfb);

	init();

	while (!screen->isExit()){
		renderer->clear();
		screen->dispatch();
		//renderer->drawPoint(50, 58, 0xffffff);

		r += 0.03f;
		Matrix mat = rotate(axis, r);
		renderer->shader.setWord(mat);

		render();

		screen->dispatch();
		screen->update();
		Sleep(1);
	}

	screen->close();
	delete screen;
	delete renderer;
	FreeConsole();
	return 0;
}