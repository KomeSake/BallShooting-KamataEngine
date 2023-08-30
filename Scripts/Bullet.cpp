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
	_isSpriteAni = false;

	switch (type) {
	case enemy_shoot:
		_type = type;
		_sprite = LoadRes::_spBullet_enemy_shoot;
		_speed = 10;
		_damage = 7;
		break;
	case player_shoot:
		_type = type;
		_sprite = LoadRes::_spBullet_player_shoot;
		_speed = 20;
		_damage = 10;
		break;
	case boss:
		_type = type;
		_isSpriteAni = true;
		_spriteAni = LoadRes::_spBullet_boss;
		_width = 128;
		_height = 128;
		_speed = 5;
		_damage = 15;
		break;
	case boss_player:
		_type = type;
		_isSpriteAni = true;
		_spriteAni = LoadRes::_spBullet_boss_player;
		_width = 128;
		_height = 128;
		_speed = 5;
		_damage = 20;
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
		if (!_isSpriteAni) {
			FrameTexture(_pos.x, _pos.y, _sprite, rad, _color);
		}
		else {
			FrameAnimation(_pos.x, _pos.y, _spriteAni, rad, _color, 100, 2);
		}
	}
}

void Bullet::Fire(Vector2 bornPos, Vector2 dir)
{
	_pos = bornPos;
	_dir = dir;
	switch (_type) {
	case normal:
		BulletManager::_bulletUpdata_player.push_back(this);
		break;
	case enemy_shoot:
		BulletManager::_bulletUpdata_enemy.push_back(this);
		break;
	case player_shoot:
		BulletManager::_bulletUpdata_player.push_back(this);
		break;
	case boss:
		BulletManager::_bulletUpdata_enemy.push_back(this);
		break;
	case boss_player:
		BulletManager::_bulletUpdata_player.push_back(this);
		break;
	}
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
	for (Bullet* element : _bulletUpdata_enemy) {
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
	case Bullet::enemy_shoot:
		if (_bulletIdiePool_enemy_shoot.empty()) {
			Bullet* bullet = new Bullet(type);
			return bullet;
		}
		else {
			Bullet* bullet = _bulletIdiePool_enemy_shoot.front();
			_bulletIdiePool_enemy_shoot.pop();
			bullet->Inital(type);
			return bullet;
		}
		break;
	case Bullet::player_shoot:
		if (_bulletIdiePool_player_shoot.empty()) {
			Bullet* bullet = new Bullet(type);
			return bullet;
		}
		else {
			Bullet* bullet = _bulletIdiePool_player_shoot.front();
			_bulletIdiePool_player_shoot.pop();
			bullet->Inital(type);
			return bullet;
		}
		break;
	case Bullet::boss:
		if (_bulletIdiePool_boss.empty()) {
			Bullet* bullet = new Bullet(type);
			return bullet;
		}
		else {
			Bullet* bullet = _bulletIdiePool_boss.front();
			_bulletIdiePool_boss.pop();
			bullet->Inital(type);
			return bullet;
		}
		break;
	case Bullet::boss_player:
		if (_bulletIdiePool_boss_player.empty()) {
			Bullet* bullet = new Bullet(type);
			return bullet;
		}
		else {
			Bullet* bullet = _bulletIdiePool_boss_player.front();
			_bulletIdiePool_boss_player.pop();
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
	case Bullet::normal: {
		auto it = find(_bulletUpdata_player.begin(), _bulletUpdata_player.end(), bullet);
		if (it != _bulletUpdata_player.end()) {
			_bulletUpdata_player.erase(it);
		}
		_bulletIdiePool_normal.push(bullet);
		break; }
	case Bullet::enemy_shoot: {
		auto it = find(_bulletUpdata_enemy.begin(), _bulletUpdata_enemy.end(), bullet);
		if (it != _bulletUpdata_enemy.end()) {
			_bulletUpdata_enemy.erase(it);
		}
		_bulletIdiePool_enemy_shoot.push(bullet);
		break; }
	case Bullet::player_shoot: {
		auto it = find(_bulletUpdata_player.begin(), _bulletUpdata_player.end(), bullet);
		if (it != _bulletUpdata_player.end()) {
			_bulletUpdata_player.erase(it);
		}
		_bulletIdiePool_normal.push(bullet);
		break; }
	case Bullet::boss: {
		auto it = find(_bulletUpdata_enemy.begin(), _bulletUpdata_enemy.end(), bullet);
		if (it != _bulletUpdata_enemy.end()) {
			_bulletUpdata_enemy.erase(it);
		}
		_bulletIdiePool_boss.push(bullet);
		break; }
	case Bullet::boss_player: {
		auto it = find(_bulletUpdata_player.begin(), _bulletUpdata_player.end(), bullet);
		if (it != _bulletUpdata_player.end()) {
			_bulletUpdata_player.erase(it);
		}
		_bulletIdiePool_boss_player.push(bullet);
		break; }
	}
}

void BulletManager::BulletShow()
{
	for (Bullet* element : _bulletUpdata_player) {
		element->Show();
		element->ToDead();
	}
	for (Bullet* element : _bulletUpdata_enemy) {
		element->Show();
		element->ToDead();
	}
}
