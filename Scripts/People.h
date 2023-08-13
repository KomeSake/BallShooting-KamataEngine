#pragma once
#include <Novice.h>
#include <math.h>
#include "LoadRes.h"
#include "Map.h"
#include "Bullet.h"

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
};

