#include "Camera.h"

Camera::Camera(Player* player)
{
	_playerObj = player;
	_screenPos = { 0,0 };

	_bgWidth = 3840;
	_bgHeight = 2160;
	_screenWidth = 1280;
	_screenHeight = 720;

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
	Animation(_bgPos.x, _bgPos.y, LoadRes::_bgSP, _bgWidth, _bgHeight, WHITE);
	Animation(_playerObj->_pos.x, _playerObj->_pos.y, _playerObj->_image, _playerObj->_width, _playerObj->_height, _playerObj->_color);
}

void Camera::Animation(float x, float y, int image, float w, float h, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2 - w / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - 720) * -1 - h / 2;
	Novice::DrawSprite((int)screenPosX, (int)screenPosY, image, 1, 1, 0, color);
}
