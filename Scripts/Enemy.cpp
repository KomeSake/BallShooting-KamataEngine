#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(EnemyType type)
{
	Inital(type);
}

void Enemy::Inital(EnemyType type)
{
	_pos = { -100,0 };
	_dir = { 0,0 };
	_vel = { 0,0 };
	_acceleration = { 0,0 };
	_speed = 2;
	_bounce = 0.7f;
	_friction = 0.97f;
	_velMax = 10;

	//_sprite = LoadRes::_spListEnemy1;
	_width = 128;
	_height = 128;
	_color = WHITE;

	_type = dog;
	_hp = 20;
	_damage = 10;
	_warningLength = 700;
	_isWarning = false;
	_hitBox_enemy = -50;
	_bounceValue_bullet = 20;

	_isHarmed = false;

	switch (type) {
	case dog:
		_type = type;
		break;
	}
}

void Enemy::Move(Vector2 playerPos)
{
	//移动前的位置
	Vector2 originalPos = _pos;

	Vector2 dir = { playerPos.x - _pos.x,playerPos.y - _pos.y };
	float vectorLength = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
	if (vectorLength != 0) {
		dir.x = dir.x / vectorLength;
		dir.y = dir.y / vectorLength;
	}
	_dir = dir;

	if (vectorLength < _warningLength) {
		_isWarning = true;
	}
	if (_isWarning) {
		//从Player类偷来的物理移动和地图碰撞检测
		float bgHeight = 3200;
		float minMapSize = 128;
		int playerCheckRow = (int)((bgHeight - _pos.y) / minMapSize);
		int playerCheckLine = (int)(_pos.x / minMapSize);

		//开始x轴的移动，移动后算出新的上下左右4个角的格子位置
		_acceleration.x = _dir.x * _speed;
		if (_vel.x<_velMax && _vel.x>-_velMax) {
			_vel.x += _acceleration.x;
		}
		_vel.x = _vel.x * _friction;
		_pos.x += _vel.x;
		int checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
		int checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);//下和右格要缩小一像素，这样才准确
		int checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
		int checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
		//通过4个角的格子来判断是否已经不在可通过格子中，如果不在就把player移回去
		if (_vel.x > 0) {
			if (!Map::IsThrough(Map::_mapData1, checkUp, checkRight)
				|| !Map::IsThrough(Map::_mapData1, checkDown, checkRight)) {
				_pos.x = playerCheckLine * minMapSize + _width / 2;
				_vel.x = _vel.x * -_bounce;
			}
		}
		else if (_vel.x < 0) {
			if (!Map::IsThrough(Map::_mapData1, checkUp, checkLeft)
				|| !Map::IsThrough(Map::_mapData1, checkDown, checkLeft)) {
				_pos.x = playerCheckLine * minMapSize + _width / 2;
				_vel.x = _vel.x * -_bounce;
			}
		}

		//y轴也是相同的，先移动后判断，如果不对劲就把player移回去
		//核心区别就在于，之前都是x轴和y轴都移动了后再进行判断，这样会导致后面的轴被前面影响而导致误判
		_acceleration.y = _dir.y * _speed;
		if (_vel.y<_velMax && _vel.y>-_velMax) {
			_vel.y += _acceleration.y;
		}
		_vel.y = _vel.y * _friction;
		_pos.y += _vel.y;
		checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
		checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);
		checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
		checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
		if (_vel.y > 0) {
			if (!Map::IsThrough(Map::_mapData1, checkUp, checkLeft)
				|| !Map::IsThrough(Map::_mapData1, checkUp, checkRight)) {
				_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_vel.y = _vel.y * -_bounce;
			}
		}
		else if (_vel.y < 0) {
			if (!Map::IsThrough(Map::_mapData1, checkDown, checkLeft)
				|| !Map::IsThrough(Map::_mapData1, checkDown, checkRight)) {
				_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_vel.y = _vel.y * -_bounce;
			}
		}
	}
	//和其他敌人的碰撞检测
	for (Enemy* element : EnemyManager::_enemyUpdateVector) {
		if (element != this) {
			float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
			if (length - _hitBox_enemy < element->_width / 2 + _width / 2) {
				_pos = originalPos;
			}
		}
	}
}

void Enemy::Fire(Vector2 bornPos)
{
	_pos = bornPos;
	EnemyManager::_enemyUpdateVector.push_back(this);
}

void Enemy::CollideSystem(Vector2 playerPos, int playerPattern, float playerBallDamage)
{
	//和子弹之间的碰撞
	for (Bullet* element : BulletManager::_bulletUpdata_player) {
		float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
		if (length + 50 < element->_width / 2 + _width / 2) {
			Vector2 hitDir = { _pos.x - element->_pos.x,_pos.y - element->_pos.y };
			hitDir = VectorNormalization(hitDir.x, hitDir.y);
			_vel.x = hitDir.x * _bounceValue_bullet;
			_vel.y = hitDir.y * _bounceValue_bullet;

			_hp -= element->_damage;
			_isHarmed = true;
			BulletManager::ReleaseBullet(element);
		}
	}
	//和球形态玩家之间的碰撞
	if (playerPattern == 0) {
		float length = sqrtf(powf(playerPos.x - _pos.x, 2) + powf(playerPos.y - _pos.y, 2));
		if (length + 50 < 128 / 2 + 128 / 2) {
			Vector2 hitDir = { _pos.x - playerPos.x,_pos.y - playerPos.y };
			hitDir = VectorNormalization(hitDir.x, hitDir.y);
			_vel.x = hitDir.x * _bounceValue_bullet;
			_vel.y = hitDir.y * _bounceValue_bullet;

			_hp -= playerBallDamage;
			_isHarmed = true;
		}
	}
}

void Enemy::Effect()
{
	//受伤效果
	if (_isHarmed) {
		_color = RED;
		if (MyTimers(100, 1)) {
			_color = WHITE;
			_isHarmed = false;
		}
	}
}

void Enemy::IsDead()
{
	int aniTime = (int)(LoadRes::_spListEnemyExplode.size());
	Novice::ScreenPrintf(10, 100, "sum:%d", aniTime);
	if (MyTimers(aniTime*100,2)) {
		EnemyManager::ReleaseEnemy(this);
	}
}







void EnemyManager::EnemyUpdata(Enemy::Vector2 playerPos, int playerPattern, float playerBallDamage)
{
	//应该要写上状态机变换才对，不应该直接就通过生命值来变换，这样不利于拓展
	//不过为了做的快，其实直接就是两个状态了，一个活着一个死亡
	for (Enemy* element : _enemyUpdateVector) {
		if (element->_hp < 0) {
			element->IsDead();
		}
		else {
			element->CollideSystem(playerPos, playerPattern, playerBallDamage);
			element->Move(playerPos);
			element->Effect();
		}
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
