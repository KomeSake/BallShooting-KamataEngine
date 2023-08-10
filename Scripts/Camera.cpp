#include "Camera.h"

Camera::Camera(Player* player, const int screenW, const int screenH, int bgW, int bgH, int minMapSize)
{
	_playerObj = player;

	_bgWidth = (float)bgW;
	_bgHeight = (float)bgH;
	_screenWidth = (float)screenW;
	_screenHeight = (float)screenH;
	_minMapSize = (float)minMapSize;

	_cameraPos = { _screenWidth / 2,_screenHeight / 2 };
	_bgPos = { _bgWidth / 2,_bgHeight / 2 };
}

void Camera::Move()
{
	if (_playerObj->_pos.x >= _screenWidth / 2 && _playerObj->_pos.x <= _bgWidth - _screenWidth / 2) {
		_cameraPos.x = _playerObj->_pos.x;
	}
	if (_playerObj->_pos.y >= _screenHeight / 2 && _playerObj->_pos.y <= _bgHeight - _screenHeight / 2) {
		_cameraPos.y = _playerObj->_pos.y;
	}
}

void Camera::Show()
{
	//Animation(_bgPos.x, _bgPos.y, LoadRes::_bgSP, _bgWidth, _bgHeight, WHITE);
	Animation(_playerObj->_pos.x, _playerObj->_pos.y, _playerObj->_image, _playerObj->_width, _playerObj->_height, _playerObj->_color);
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
				Animation(minMapPos.x, minMapPos.y, LoadRes::_map_wall, minSize, minSize, WHITE);
				break;
			case 'o':
				Animation(minMapPos.x, minMapPos.y, LoadRes::_map_floor, minSize, minSize, WHITE);
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}

void Camera::Animation(float x, float y, int image, float w, float h, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2 - w / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - _screenHeight) * -1 - h / 2;
	Novice::DrawSprite((int)screenPosX, (int)screenPosY, image, 1, 1, 0, color);
}