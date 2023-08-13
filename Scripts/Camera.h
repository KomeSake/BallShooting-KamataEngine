#pragma once
#include <vector>
#include "Player.h"
#include "Bullet.h"

using namespace std;

class Camera
{
public:
	struct Vector2
	{
		float x;
		float y;
	};

	Vector2 _cameraPos;
	Vector2 _bgPos;

	float _bgWidth;
	float _bgHeight;
	float _screenWidth;
	float _screenHeight;
	float _minMapSize;

	Camera(const int screenW, const int screenH, int bgW, int bgH, int minMapSize);
	void Move(Vector2 playerPos);
	void Show(Player* PlayerObj);
	void MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize);
	void BulletShow();

	//相机设置步骤
	//1.所有坐标都必须是世界坐标
	//2.设置一个相机的坐标
	//3.将相机框选出的部分转换成相机坐标系
	//4.将框选出的部分转换成屏幕坐标系
	void FrameAnimation(float x, float y, int image, float w, float h, int color);

	//将屏幕坐标变成世界坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY);
};