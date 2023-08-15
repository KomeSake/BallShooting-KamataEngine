#pragma once
#include <vector>
#include <cmath>
#include <map>
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

	void FrameTexture(float x, float y, LoadRes::Sprite sprite, int color);
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, float rad, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, int color);
	Vector2 AditionRule(Vector2 pos, float rad);//加法定理，为了计算旋转后的图像位置


	//将屏幕坐标变成世界坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY);
	//将世界坐标变成屏幕坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 WorldToScreen(Vector2 worldPos, Vector2 cameraPos);
};