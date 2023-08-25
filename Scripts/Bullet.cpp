#include "Bullet.h"

Bullet::Bullet(BulletType type)
{
	Inital(type);
}

void Bullet::Inital(BulletType type)
{
	_pos = { 0,-100 };
	_dir = { 0,0 };
	_speed = 30;
	_sprite = LoadRes::_spBullet;
	_width = 64;
	_height = 64;
	_color = WHITE;

	_type = normal;
	_isAlive = true;
	_damage = 2;

	_toDeadEffect = false;

	switch (type) {
	case normal:
		break;
	}
}

void Bullet::Move(Vector2 cameraPos, vector<vector<char>> mapData, float bgHeight, float minMapSize)
{
	if (_isAlive) {
		//是否在屏幕内
		float screenPosX = _pos.x - cameraPos.x + 1920.f / 2 - _width / 2;
		float screenPosY = (_pos.y - cameraPos.y + 1080.f / 2 - 1080) * -1 - _height / 2;
		if (screenPosX > -100 && screenPosX < 1920 + 100
			&& screenPosY > -100 && screenPosY < 1080 + 100) {
			_pos.x += _dir.x * _speed;
			_pos.y += _dir.y * _speed;
		}
		else {
			BulletManager::ReleaseBullet(this);
		}
		//判断是否碰到了不可通过格子
		int posCheckLine = (int)((bgHeight - _pos.y) / minMapSize);
		int posCheckRow = (int)(_pos.x / minMapSize);
		if (!Map::IsThrough(mapData, posCheckLine, posCheckRow)) {
			_isAlive = false;
		}
	}
}

void Bullet::Show()
{
	if (_isAlive) {
		float rad = SpriteToObjDir(Vector2{ _dir.x, _dir.y });
		FrameTexture(_pos.x, _pos.y, _sprite, rad, _color);
	}
}

void Bullet::Fire(Vector2 bornPos, Vector2 dir)
{
	_pos = bornPos;
	_dir = dir;
	BulletManager::_bulletUpdata_player.push_back(this);
}

void Bullet::ToDead()
{
	if (!_isAlive) {
		srand(unsigned int(time(nullptr)));//随机数计算
		float rad = (float)(rand() % 10);//随机个角度出来
		FrameAnimation(_pos.x, _pos.y, LoadRes::_spListBulletExplode, rad, _color, 100, 1);
		if (MyTimers(399, 1)) {
			//爆炸特效一共4张，少1帧减少卡帧现象
			BulletManager::ReleaseBullet(this);
		}
	}
}

void BulletManager::BulletUpdata(Bullet::Vector2 cameraPos, vector<vector<char>> mapData, float bgHeight, float minMapSize)
{
	for (Bullet* element : _bulletUpdata_player) {
		element->Move(cameraPos, mapData, bgHeight, minMapSize);
	}
}

Bullet* BulletManager::AcquireBullet(Bullet::BulletType type)
{
	switch (type) {
	case Bullet::normal:
		if (_bulletIdiePool_normal.empty()) {
			Bullet* bullet = new Bullet(type);
			return bullet;
		}
		else {
			Bullet* bullet = _bulletIdiePool_normal.front();
			_bulletIdiePool_normal.pop();
			bullet->Inital(type);
			return bullet;
		}
		break;
	}
	return nullptr;
}

void BulletManager::ReleaseBullet(Bullet* bullet)
{
	switch (bullet->_type) {
	case Bullet::normal:
		auto it = find(_bulletUpdata_player.begin(), _bulletUpdata_player.end(), bullet);
		if (it != _bulletUpdata_player.end()) {
			_bulletUpdata_player.erase(it);
		}
		_bulletIdiePool_normal.push(bullet);
		break;
	}
}

void BulletManager::BulletShow()
{
	for (Bullet* element : _bulletUpdata_player) {
		element->Show();
		element->ToDead();
	}
}
