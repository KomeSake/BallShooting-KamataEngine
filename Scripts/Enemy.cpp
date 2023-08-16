#include "Enemy.h"

Enemy::Enemy(EnemyType type)
{
	Inital(type);
}

void Enemy::Inital(EnemyType type)
{
	_pos = { -100,0 };
	_dir = { 0,0 };
	_speed = 10;
	_sprite = LoadRes::_spEnemy1;
	_width = 96;
	_height = 96;
	_color = WHITE;
	_type = dog;
	_damage = 10;
	_warningLength = 700;
	switch (type) {
	case dog:
		_type = type;
		break;
	}
}

void Enemy::Move(Vector2 playerPos)
{
	Vector2 dir = { playerPos.x - _pos.x,playerPos.y - _pos.y };
	float vectorLength = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
	if (vectorLength != 0) {
		dir.x = dir.x / vectorLength;
		dir.y = dir.y / vectorLength;
	}
	_dir = dir;

	if (vectorLength < _warningLength) {
		_pos.x += dir.x * _speed;
		_pos.y += dir.y * _speed;
	}
}

void Enemy::Fire(Vector2 bornPos)
{
	_pos = bornPos;
	EnemyManager::_enemyUpdateVector.push_back(this);
}

void EnemyManager::EnemyUpdata(Enemy::Vector2 playerPos)
{
	for (Enemy* element : _enemyUpdateVector) {
		element->Move(playerPos);
	}
}

Enemy* EnemyManager::AcquireEnemy(Enemy::EnemyType type)
{
	switch (type) {
	case Enemy::dog:
		if (_enemyIdiePool_dog.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_dog.front();
			_enemyIdiePool_dog.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	}
	return nullptr;
}

void EnemyManager::ReleaseEnemy(Enemy* enemy)
{
	switch (enemy->_type) {
	case Enemy::dog:
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_dog.push(enemy);
		break;
	}
}

void EnemyManager::EnemyBornToMap(vector<vector<char>> mapData, float bgW, float bgH, float minSize)
{
	bgW;
	Enemy::Vector2 minMapPos = { 0,0 };
	minMapPos.x = minSize / 2;
	minMapPos.y = bgH - minSize / 2;
	for (const auto& row : mapData) {
		for (const char& line : row) {
			switch (line) {
			case 'e':
				Enemy * emeny = EnemyManager::AcquireEnemy(Enemy::dog);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}
