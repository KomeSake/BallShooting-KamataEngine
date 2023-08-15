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
	_type = normal;
	_sprite = LoadRes::_spBullet;
	_width = 64;
	_height = 64;
	_color = WHITE;

	switch (type) {
	case normal:
		break;
	}
}

void Bullet::Move(Vector2 cameraPos)
{
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
}

void Bullet::Fire(Vector2 pos, Vector2 dir)
{
	_pos = pos;
	_dir = dir;
	BulletManager::_bulletUpdata_player.push_back(this);
}

void BulletManager::BulletUpdata(Bullet::Vector2 cameraPos)
{
	for (Bullet* element : _bulletUpdata_player) {
		element->Move(cameraPos);
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
