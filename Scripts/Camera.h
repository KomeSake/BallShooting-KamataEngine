#pragma once
#include "Player.h"
#include <vector>

using namespace std;

class Camera
{
	struct Vector2
	{
		float x;
		float y;
	};

public:
	Vector2 _cameraPos;
	Vector2 _bgPos;

	float _bgWidth;
	float _bgHeight;
	float _screenWidth;
	float _screenHeight;
	float _minMapSize;

	Player* _playerObj;

	Camera(Player* player, const int screenW, const int screenH, int bgW, int bgH, int minMapSize);
	void Move();
	void Show();
	void MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize);

	//相机设置步骤
	//1.所有坐标都必须是世界坐标
	//2.设置一个相机的坐标
	//3.将相机框选出的部分转换成相机坐标系
	//4.将框选出的部分转换成屏幕坐标系
	void Animation(float x, float y, int image, float w, float h, int color);
};