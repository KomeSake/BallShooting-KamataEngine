﻿#include "Camera.h"

Camera::Camera(const int screenW, const int screenH, int bgW, int bgH, int minMapSize)
{
	_bgWidth = (float)bgW;
	_bgHeight = (float)bgH;
	_screenWidth = (float)screenW;
	_screenHeight = (float)screenH;
	_minMapSize = (float)minMapSize;

	_cameraPos = { _screenWidth / 2,_screenHeight / 2 };
	_bgPos = { _bgWidth / 2,_bgHeight / 2 };
}

void Camera::Move(Vector2 playerPos)
{
	if (playerPos.x >= _screenWidth / 2 && playerPos.x <= _bgWidth - _screenWidth / 2) {
		_cameraPos.x = playerPos.x;
	}
	if (playerPos.y >= _screenHeight / 2 && playerPos.y <= _bgHeight - _screenHeight / 2) {
		_cameraPos.y = playerPos.y;
	}
}

void Camera::Show(Player* PlayerObj)
{
	FrameAnimation(PlayerObj->_pos.x, PlayerObj->_pos.y, PlayerObj->_image, PlayerObj->_width, PlayerObj->_height, PlayerObj->_color);
}

void Camera::MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize) {
	bgW;
	Vector2 minMapPos = { 0,0 };
	minMapPos.x = minSize / 2;
	minMapPos.y = bgH - minSize / 2;
	for (const auto& row : mapData) {
		for (const char& line : row) {
			switch (line) {
			case 'w':
				FrameAnimation(minMapPos.x, minMapPos.y, LoadRes::_map_wall, minSize, minSize, WHITE);
				break;
			case 'o':
				FrameAnimation(minMapPos.x, minMapPos.y, LoadRes::_map_floor, minSize, minSize, WHITE);
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}

void Camera::BulletShow() {
	for (Bullet* element : BulletManager::_bulletUpdata_player) {
		//角度问题实在是太难了，如果是归一化后的向量，直接tan即可求出弧度(因为长度变成1了)
		//但是这个弧度是和x轴进行比较的，所以还需要减去90度来转到y轴
		//因为弧度实在不容易看出来问题，所以我全部转成了degree，然后输出的时候在转回弧度
		//注意！！数学上，角度增加应该是逆时针旋转，但是我发现引擎是顺时针旋转，所以我的函数也是顺时针
		//所以如果是数学上算出来的角度，使用旋转函数时候必须换成负数！
		float rad = atan2f(element->_dir.y, element->_dir.x);
		float degree = 180.f / acosf(-1) * rad;
		if (degree < 0) {
			degree += 360;
		}
		degree -= 90;
		FrameAnimation(element->_pos.x, element->_pos.y, element->_image, element->_width, element->_height, -(acosf(-1) / 180 * degree), element->_color);
	}
}

void Camera::FrameAnimation(float x, float y, int image, float w, float h, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - _screenHeight) * -1;
	Novice::DrawSprite((int)(screenPosX - w / 2), (int)(screenPosY - h / 2), image, 1, 1, 0, color);
}

void Camera::FrameAnimation(float x, float y, int image, float w, float h, float rad, int color)
{
	Vector2 rotated = { -w / 2,h / 2 };
	//RotatedPos
	rotated = AditionRule(rotated, -rad);
	//ScreenPos
	rotated = { rotated.x + x ,rotated.y + y };
	rotated = WorldToScreen(rotated, _cameraPos);

	Novice::DrawSprite((int)rotated.x, (int)rotated.y, image, 1, 1, rad, color);
}

Camera::Vector2 Camera::AditionRule(Vector2 pos, float rad)
{
	float aditionX = pos.x * cosf(rad) - pos.y * sinf(rad);
	float aditionY = pos.y * cosf(rad) + pos.x * sinf(rad);
	return Vector2{ aditionX,aditionY };
}

Camera::Vector2 Camera::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY) {
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}

Camera::Vector2 Camera::WorldToScreen(Vector2 worldPos, Vector2 cameraPos)
{
	float screenPosX = worldPos.x - cameraPos.x + 1920.f / 2;
	float screenPosY = (worldPos.y - cameraPos.y + 1080.f / 2 - 1080) * -1;
	return Vector2(screenPosX, screenPosY);
}
