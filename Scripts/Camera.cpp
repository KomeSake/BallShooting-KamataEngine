#include "Camera.h"

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
		FrameAnimation(element->_pos.x, element->_pos.y, element->_image, element->_width, element->_height, element->_color);
	}
}

void Camera::FrameAnimation(float x, float y, int image, float w, float h, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2 - w / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - _screenHeight) * -1 - h / 2;
	Novice::DrawSprite((int)screenPosX, (int)screenPosY, image, 1, 1, 0, color);
}

Camera::Vector2 Camera::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY) {
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}