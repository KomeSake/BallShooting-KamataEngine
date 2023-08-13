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

	_mousePos.x = 0;
	_mousePos.y = 0;
	_bulletPos = { 0,0 };
	_bulletDir = { 0,0 };
	_bulletSpeed = 30;
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

	//格子限制
	//注意！地图格子是二维容器装着的，所以序号是从0开始

	//先计算出当前所在的格子
	int playerCheckRow = (int)((bgHeight - _pos.y) / minMapSize);
	int playerCheckLine = (int)(_pos.x / minMapSize);

	//开始x轴的移动，移动后算出新的上下左右4个角的格子位置
	_pos.x += _dir.x * _speed;
	int checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
	int checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);//下和右格要缩小一像素，这样才准确
	int checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
	int checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
	//通过4个角的格子来判断是否已经不在可通过格子中，如果不在就把player移回去
	if (_dir.x > 0) {
		if (Map::_mapData1[checkUp][checkRight] != 'o'
			|| Map::_mapData1[checkDown][checkRight] != 'o') {
			_pos.x = playerCheckLine * minMapSize + _width / 2;
		}
	}
	else if (_dir.x < 0) {
		if (Map::_mapData1[checkUp][checkLeft] != 'o'
			|| Map::_mapData1[checkDown][checkLeft] != 'o') {
			_pos.x = playerCheckLine * minMapSize + _width / 2;
		}
	}

	//y轴也是相同的，先移动后判断，如果不对劲就把player移回去
	//核心区别就在于，之前都是x轴和y轴都移动了后再进行判断，这样会导致后面的轴被前面影响而导致误判
	_pos.y += _dir.y * _speed;
	checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
	checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);
	checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
	checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
	if (_dir.y > 0) {
		if (Map::_mapData1[checkUp][checkLeft] != 'o'
			|| Map::_mapData1[checkUp][checkRight] != 'o') {
			_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
		}
	}
	else if (_dir.y < 0) {
		if (Map::_mapData1[checkDown][checkLeft] != 'o'
			|| Map::_mapData1[checkDown][checkRight] != 'o') {
			_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
		}
	}
	//Novice::ScreenPrintf(10, 100, "u:%d,d:%d,l:%d,r:%d,", checkUp, checkDown, checkLeft, checkRight);

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

void Player::Attack(Vector2 cameraPos)
{
	int x = 0, y = 0;
	Novice::GetMousePosition(&x, &y);
	Vector2 worldPos = ScreenToWorld((float)x, (float)y, cameraPos.x, cameraPos.y);
	_mousePos.x = worldPos.x;
	_mousePos.y = worldPos.y;

	_bulletDir = { _mousePos.x - _pos.x,_mousePos.y - _pos.y };
	float vectorLength = sqrtf(powf(_bulletDir.x, 2) + powf(_bulletDir.y, 2));
	if (vectorLength != 0) {
		_bulletDir.x = _bulletDir.x / vectorLength;
		_bulletDir.y = _bulletDir.y / vectorLength;
	}

	if (Novice::IsTriggerMouse(0)) {
		Bullet* bullet = BulletManager::AcquireBullet(Bullet::normal);
		bullet->Fire(Bullet::Vector2{_pos.x, _pos.y}, Bullet::Vector2{_bulletDir.x, _bulletDir.y});
	}
}
