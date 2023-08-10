#include "Player.h"

Player::Player()
{
	_pos = { 1280 / 2 - 100,720 / 2 - 100 };
	_dir = { 0,0 };
	_vel = { 0,0 };
	_speed = 10;
	_image = LoadRes::_playerSP;
	_width = 128;
	_height = 128;
	_color = WHITE;
}

void Player::Move(char keys[], float bgWidth, float bgHeight, float minMapSize)
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

	//格子限制
	////注意！地图格子是二维容器装着的，所以序号是从0开始
	int checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
	int checkDown = (int)((bgHeight - _pos.y + _height / 2) / minMapSize);
	int checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
	int checkRight = (int)((_pos.x + _width / 2) / minMapSize);
	int playerCheckRow = (int)((bgHeight - _pos.y) / minMapSize);
	int playerCheckLine = (int)(_pos.x / minMapSize);

	Novice::ScreenPrintf(10, 100, "%d,%d,%d,%d,", checkUp, checkDown, checkLeft, checkRight);

	if (Map::_mapData1[checkUp][playerCheckLine] != 'o') {
		_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
	}
	else if (Map::_mapData1[checkDown][playerCheckLine] != 'o') {
		_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
	}
	if (Map::_mapData1[playerCheckRow][checkLeft] != 'o') {
		_pos.x = playerCheckLine * minMapSize + _width / 2;
	}
	else if (Map::_mapData1[playerCheckRow][checkRight] != 'o') {
		_pos.x = playerCheckLine * minMapSize + _width / 2;
	}


	//最外围的边界限制
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