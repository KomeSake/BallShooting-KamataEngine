#include "Player.h"

Player::Player()
{
	_pos = { 1280 / 2 - 100,720 / 2 - 100 };
	_dir = { 0,0 };
	_vel = { 0,0 };
	_speed = 10;
	_image = LoadRes::_playerSP;
	_width = 64;
	_height = 64;
	_color = WHITE;
}

void Player::Move(char keys[], float bgWidth, float bgHeight)
{
	//移动部分
	if (keys[DIK_W]) {
		_dir.y = 1;
	}
	else if (keys[DIK_S]) {
		_dir.y = -1;
	}
	else {
		_dir.y = 0;
	}
	if (keys[DIK_D]) {
		_dir.x = 1;
	}
	else if (keys[DIK_A]) {
		_dir.x = -1;
	}
	else {
		_dir.x = 0;
	}
	float vectorLength = sqrtf(powf(_dir.x, 2) + powf(_dir.y, 2));
	if (vectorLength != 0) {
		_dir.x = _dir.x / vectorLength;
		_dir.y = _dir.y / vectorLength;
	}
	_pos.x += _dir.x * _speed;
	_pos.y += _dir.y * _speed;
	//限制部分
	if (_pos.x + _width / 2 > bgWidth) {
		_pos.x = bgWidth - _width / 2;
	}
	else if (_pos.x - _width / 2 < 0) {
		_pos.x = _width / 2;
	}
	if (_pos.y + _height / 2 > bgHeight) {
		_pos.y = bgHeight - _height / 2;
	}
	else if (_pos.y - _height / 2 < 0) {
		_pos.y = _height / 2;
	}
}

