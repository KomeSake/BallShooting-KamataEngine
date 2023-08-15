#pragma once
#include <Novice.h>
#include <cmath>
#include <ctime>
#include "LoadRes.h"
#include "Map.h"
#include "Bullet.h"
using namespace std;

class People
{
public:
	struct Vector2
	{
		float x;
		float y;
	};

	//将屏幕坐标变成世界坐标(为了方便，屏幕大小已定为1920*1080，从Camera类偷来的)
	Vector2 ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY);

	//计时器
	int MyTimers(int milli, int index);
	clock_t _timeStart[31] = { 0 };
	clock_t _timeEnd[31] = { 0 };
	bool _isTimeOpen[31] = { 0 };
};

