#include "Enemy.h"

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

	_sprite = LoadRes::_spListEnemy1;
	_width = 100;
	_height = 100;
	_scaleX = 1;
	_scaleY = 1;
	_color = WHITE;

	_type = dog;
	_hp = 10;
	_damage = 5;

	_isRunToPlayer = true;
	_toPlayerLengthStop = 0;
	_shootTimer = 0;

	_isWarning = false;
	_warningLength = 850;

	_isCrash = false;
	_crashScaleSpeed = 0.01f;
	_crashScaleMax = 1.2f;
	_crashScaleReset = _scaleX;

	_hitBox_enemy = -50;
	_bounceValue_bullet = 10;
	_bounceValue_player = 35;

	_isDrop = false;
	_isHarmed = false;
	_isDead = false;

	_bossPattern = 0;
	_bossSplit = 0;
	_isBossDead = false;
	_isBossCrash = false;
	_isBossSplit = false;
	_isBoss2Init = false;

	switch (type) {
	case dog:
		_type = type;
		break;
	case dog1_1:
		_sprite = LoadRes::_spListEnemy1_1;
		_width = 128;
		_height = 128;
		_scaleX = 1.2f;
		_scaleY = 1.2f;
		_type = type;
		_hp = 40;
		_damage = 17;
		_warningLength = 500;
		_bounce = 0.5f;
		_velMax = 10;
		_bounceValue_bullet = 1;
		_bounceValue_player = 10;
		_crashScaleMax = 1.4f;
		_crashScaleReset = _scaleX;
		break;
	case dog2:
		_sprite = LoadRes::_spListEnemy2;
		_width = 128;
		_height = 128;
		_type = type;
		_hp = 20;
		_damage = 13;
		_warningLength = 700;
		_bounce = 0.5f;
		_velMax = 7;
		_bounceValue_bullet = 5;
		_bounceValue_player = 20;
		break;
	case shoot:
		_sprite = LoadRes::_spListEnemy3;
		_width = 118;
		_height = 118;
		_type = type;
		_hp = 7;
		_damage = 10;
		_shootTimer = 500;
		_warningLength = 1000;
		_toPlayerLengthStop = 700;
		_bounce = 0.5f;
		_velMax = 5;
		_bounceValue_bullet = 3;
		_bounceValue_player = 30;
		break;
	case doubleShoot:
		_sprite = LoadRes::_spListEnemy4;
		_width = 128;
		_height = 128;
		_type = type;
		_hp = 20;
		_damage = 20;
		_shootTimer = 700;
		_warningLength = 800;
		_toPlayerLengthStop = 800;
		_bounce = 0.5f;
		_velMax = 3;
		_bounceValue_bullet = 8;
		_bounceValue_player = 15;
		break;
	case boss:
	case boss2:
		_isBossDead = false;
		_sprite = LoadRes::_spListEnemy_Boss;
		_width = 512;
		_height = 512;
		_type = type;
		_hp = 200;
		_damage = 10;
		_shootTimer = 700;
		_warningLength = 2000;
		_toPlayerLengthStop = 700;
		_bounce = 0.5f;
		_velMax = 3;
		_bounceValue_bullet = 1;
		_bounceValue_player = 15;
		_crashScaleMax = 1.1f;
		break;
	}
	_hpMax = _hp;
}

void Enemy::Move(Vector2 playerPos, vector<vector<char>> mapData, float bgHeight, float minMapSize)
{
	//移动前的位置
	Vector2 originalPos = _pos;

	Vector2 dir = { 0,0 };
	if (!_isDrop) {
		dir = { playerPos.x - _pos.x,playerPos.y - _pos.y };
	}
	float vectorLength = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
	if (vectorLength < _toPlayerLengthStop) {
		_isRunToPlayer = false;
	}
	else {
		_isRunToPlayer = true;
	}
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
		//int playerCheckRow = (int)((bgHeight - _pos.y) / minMapSize);
		//int playerCheckLine = (int)(_pos.x / minMapSize);

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
			if (!Map::IsThrough(mapData, checkUp, checkRight)
				|| !Map::IsThrough(mapData, checkDown, checkRight)) {
				//_pos.x = playerCheckLine * minMapSize + _width / 2;
				_pos.x = originalPos.x;
				_vel.x = _vel.x * -_bounce;
			}
		}
		else if (_vel.x < 0) {
			if (!Map::IsThrough(mapData, checkUp, checkLeft)
				|| !Map::IsThrough(mapData, checkDown, checkLeft)) {
				//_pos.x = playerCheckLine * minMapSize + _width / 2;
				_pos.x = originalPos.x;
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
			if (!Map::IsThrough(mapData, checkUp, checkLeft)
				|| !Map::IsThrough(mapData, checkUp, checkRight)) {
				//_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_pos.y = originalPos.y;
				_vel.y = _vel.y * -_bounce;
			}
		}
		else if (_vel.y < 0) {
			if (!Map::IsThrough(mapData, checkDown, checkLeft)
				|| !Map::IsThrough(mapData, checkDown, checkRight)) {
				//_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_pos.y = originalPos.y;
				_vel.y = _vel.y * -_bounce;
			}
		}

		if (!_isRunToPlayer) {
			_pos = originalPos;
		}
	}
	//和其他敌人的碰撞检测
	for (Enemy* element : EnemyManager::_enemyUpdateVector) {
		if (element != this && !element->_isDead) {
			float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
			if (length - _hitBox_enemy < element->_width / 2 + _width / 2) {
				_pos = originalPos;
				//如果在被撞飞途中碰到友方就立马结束特效
				if (_isCrash) {
					_scaleX = _crashScaleReset;
					_scaleY = _crashScaleReset;
					_crashScaleSpeed *= -1;
					_isCrash = false;
				}
			}
		}
	}
}

void Enemy::Fire(Vector2 bornPos)
{
	_pos = bornPos;
	EnemyManager::_enemyUpdateVector.push_back(this);
}

void Enemy::CollideSystem()
{
	if (!_isDrop) {
		//和子弹之间的碰撞
		for (Bullet* element : BulletManager::_bulletUpdata_player) {
			float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
			if (length + 50 < element->_width / 2 + _width / 2) {
				Vector2 hitDir = { _pos.x - element->_pos.x,_pos.y - element->_pos.y };
				hitDir = VectorNormalization(hitDir.x, hitDir.y);
				_vel.x = hitDir.x * _bounceValue_bullet;
				_vel.y = hitDir.y * _bounceValue_bullet;
				//判断前必须查看子弹是否存活，存活才和他进行碰撞反应
				if (element->_isAlive) {
					element->_isAlive = false;
					_hp -= element->_damage;
					_isHarmed = true;
				}
			}
		}
		//和球形态玩家之间的碰撞(因为Player类中也会进行此计算，所以现在直接移过去了)
	}
}

void Enemy::ShootAttack()
{
	if (!_isRunToPlayer && !_isDrop) {
		if (MyTimers(_shootTimer, 4)) {
			switch (_type) {
			case shoot: {
				Bullet* bullet = BulletManager::AcquireBullet(Bullet::enemy_shoot);
				bullet->Fire(Bullet::Vector2{ _pos.x, _pos.y }, Bullet::Vector2{ _dir.x, _dir.y });
				break; }
			case doubleShoot: {
				Bullet* bullet = BulletManager::AcquireBullet(Bullet::enemy_shoot);
				bullet->Fire(Bullet::Vector2{ _pos.x, _pos.y }, Bullet::Vector2{ _dir.x, _dir.y });
				Bullet* bullet2 = BulletManager::AcquireBullet(Bullet::enemy_shoot);
				bullet2->Fire(Bullet::Vector2{ _pos.x + 30, _pos.y + 30 }, Bullet::Vector2{ _dir.x, _dir.y });
				Bullet* bullet3 = BulletManager::AcquireBullet(Bullet::enemy_shoot);
				bullet3->Fire(Bullet::Vector2{ _pos.x - 30, _pos.y - 30 }, Bullet::Vector2{ _dir.x, _dir.y });
				break; }
			case boss: {
				Bullet* bullet = BulletManager::AcquireBullet(Bullet::boss);
				bullet->Fire(Bullet::Vector2{ _pos.x + _dir.x, _pos.y + _dir.y }, Bullet::Vector2{ _dir.x, _dir.y });
				break; }
			case boss2: {
				Bullet* bullet = BulletManager::AcquireBullet(Bullet::boss);
				bullet->Fire(Bullet::Vector2{ _pos.x + _dir.x, _pos.y + _dir.y }, Bullet::Vector2{ _dir.x, _dir.y });
				break; }
			}
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
	//被撞击效果
	if (_isCrash && !_isDrop) {
		if (_scaleX > _crashScaleMax) {
			_crashScaleSpeed *= -1;
		}
		_scaleX += _crashScaleSpeed;
		_scaleY += _crashScaleSpeed;
		if (_scaleX <= _crashScaleReset || _scaleY <= _crashScaleReset) {
			_scaleX = _crashScaleReset;
			_scaleY = _crashScaleReset;
			_crashScaleSpeed *= -1;
			_isCrash = false;
			//在Move方法中的和队友碰撞也会变成false，不然碰到队友还在放大缩小有点怪
		}
	}
}

void Enemy::ToDead()
{
	_isDead = true;
	_color = RED;
	int aniTime = (int)(LoadRes::_spListEnemyExplode.size());
	if (MyTimers(aniTime * 100, 3)) {
		EnemyManager::ReleaseEnemy(this);
	}
}

void Enemy::Show(bool isHpLine)
{
	if (_isWarning) {
		float rad = SpriteToObjDir(Vector2{ _dir.x, _dir.y });
		if (_isDrop) {
			//跌落的时候因为不追玩家了，所以角度也没了。随机一个角度出来显得不一样
			srand(unsigned int(time(nullptr)));
			rad = (float)(rand() % 10);
		}
		FrameAnimation(_pos.x, _pos.y, _sprite, _scaleX, _scaleY, rad, _color, 100, 1);
	}
	else {
		srand(unsigned int(time(nullptr)));//随机数计算
		float rad = (float)(rand() % 10);//随机个角度出来，让敌人的未警戒状态看着自然一些
		FrameAnimation(_pos.x, _pos.y, _sprite, _scaleX, _scaleY, rad, _color, 100, 1);
	}
	if (_hp <= 0) {
		float rad = SpriteToObjDir(Vector2{ _dir.x, _dir.y });
		FrameAnimation(_pos.x, _pos.y, LoadRes::_spListEnemyExplode, rad, WHITE, 100, 2);
	}
	//头顶上的血条
	if (!_isDrop && isHpLine && _type != boss) {
		LoadRes::Sprite sprite = { Novice::LoadTexture("white1x1.png") ,1,1 };
		float hpSpriteW = 60;
		float hpRate = hpSpriteW / _hpMax;
		if (_hp > 0) {
			FrameTexture_OLD(_pos.x - _width / 2 + hpSpriteW / 2, _pos.y + _height / 2 + 20, sprite, hpSpriteW, 10, 0x263238ff);
			FrameTexture_OLD(_pos.x - _width / 2 + hpSpriteW / 2, _pos.y + _height / 2 + 20, sprite, _hp * hpRate, 10, 0xe84e40ff);
		}
		else {
			FrameTexture_OLD(_pos.x - _width / 2 + hpSpriteW / 2, _pos.y + _height / 2 + 20, sprite, hpSpriteW, 10, 0x263238ff);
		}
	}
	if (!_isDrop && isHpLine && _type == boss) {
		LoadRes::Sprite sprite = { Novice::LoadTexture("white1x1.png") ,1,1 };
		float hpSpriteW = 1200;
		float hpRate = hpSpriteW / _hpMax;
		if (_hp > 0) {
			Novice::DrawSprite(int(1920.f / 2 - hpSpriteW / 2), 50, sprite.path, hpSpriteW, 50, 0, 0x263238ff);
			Novice::DrawSprite(int(1920.f / 2 - hpSpriteW / 2), 50, sprite.path, _hp * hpRate, 50, 0, 0xe84e40ff);
		}
		else {
			Novice::DrawSprite(int(1920.f / 2 - hpSpriteW / 2), 50, sprite.path, hpSpriteW, 50, 0, 0x263238ff);
		}
		//血条边框
		Novice::DrawBox(int(1920.f / 2 - hpSpriteW / 2) - 1, 50 - 1, int(hpSpriteW) + 2, 50 + 2, 0, WHITE, kFillModeWireFrame);
		Novice::DrawBox(int(1920.f / 2 - hpSpriteW / 2) - 2, 50 - 2, int(hpSpriteW) + 4, 50 + 4, 0, WHITE, kFillModeWireFrame);
		Novice::DrawBox(int(1920.f / 2 - hpSpriteW / 2) - 3, 50 - 3, int(hpSpriteW) + 6, 50 + 6, 0, WHITE, kFillModeWireFrame);
		Novice::DrawBox(int(1920.f / 2 - hpSpriteW / 2) - 4, 50 - 4, int(hpSpriteW) + 8, 50 + 8, 0, WHITE, kFillModeWireFrame);
	}
}

void Enemy::DropSystem(vector<vector<char>> mapData, float bgHeight, float minMapSize)
{
	//算出中心坐标的格子
	int posCheckLine = (int)((bgHeight - _pos.y) / minMapSize);
	int posCheckRow = (int)(_pos.x / minMapSize);

	float scaleSpeed = 0.03f;//缩小速度
	if (mapData[posCheckLine][posCheckRow] == '*') {
		_isDrop = true;
	}
	if (_isDrop) {
		_scaleX -= scaleSpeed;
		_scaleY -= scaleSpeed;
		if (_scaleX < 0 || _scaleY < 0) {
			EnemyManager::ReleaseEnemy(this);
		}
	}
}

void Enemy::EnemyBoss()
{
	//这一趴写的很乱，因为是通宵了一晚上写的deadline效果了，所以就先这样屎山吧

	//是否击败Boss
	if (_hp <= 0) {
		_isBossDead = true;
	}
	//状态变换
	if (_isBossCrash) {
		switch (_bossPattern)
		{
		case 0:
			//变成近战的属性
			_bossPattern = 1;
			_sprite = LoadRes::_spListEnemy_Boss2;
			_toPlayerLengthStop = 0;
			_bounce = 0.5f;
			_velMax = 10;
			_bounceValue_bullet = 10;
			_bounceValue_player = 25;
			break;
		case 1:
			//变成远程的属性
			_bossPattern = 0;
			_sprite = LoadRes::_spListEnemy_Boss;
			srand(unsigned int(time(nullptr)));
			float length = (float)(rand() % 700 + 400);
			_toPlayerLengthStop = length;
			_bounce = 0.5f;
			_velMax = 3;
			_bounceValue_bullet = 1;
			_bounceValue_player = 15;
			break;
		}
		//不知道为什么被撞了之后，会恢复原本的大小，所以根据分裂阶段，再重新赋值一次好了
		switch (_bossSplit) {
		case 1:
			_width = 512.f * 0.7f;
			_height = 512.f * 0.7f;
			_scaleX = 0.7f;
			_scaleY = 0.7f;
			_crashScaleMax = 0.9f;
			break;
		case 2:
			_width = 512.f * 0.4f;
			_height = 512.f * 0.4f;
			_scaleX = 0.4f;
			_scaleY = 0.4f;
			_crashScaleMax = 0.6f;
			break;
		case 3:
			_width = 512.f * 0.2f;
			_height = 512.f * 0.2f;
			_scaleX = 0.2f;
			_scaleY = 0.2f;
			_crashScaleMax = 0.4f;
			break;
		}
		_isBossCrash = false;
	}
	//分裂
	if (_hp <= _hpMax * 0.85f && _bossSplit == 0) {
		_width = 512.f * 0.7f;
		_height = 512.f * 0.7f;
		_scaleX = 0.7f;
		_scaleY = 0.7f;
		_crashScaleMax = 0.9f;
		_bossSplit = 1;
		_isBossSplit = true;
	}
	if (_hp <= _hpMax * 0.6f && _bossSplit == 1) {
		_width = 512.f * 0.4f;
		_height = 512.f * 0.4f;
		_scaleX = 0.4f;
		_scaleY = 0.4f;
		_crashScaleMax = 0.6f;
		_bossSplit = 2;
		_isBossSplit = true;
	}
	if (_hp <= _hpMax * 0.4f && _bossSplit == 2) {
		_width = 512.f * 0.2f;
		_height = 512.f * 0.2f;
		_scaleX = 0.2f;
		_scaleY = 0.2f;
		_crashScaleMax = 0.4f;
		_bossSplit = 3;
		_isBossSplit = true;
	}
	if (_type == boss2 && !_isBoss2Init) {
		//给分裂出来的boss2使用的，分裂阶段属性初始化
		switch (_bossSplit) {
		case 1:
			_width = 512.f * 0.7f;
			_height = 512.f * 0.7f;
			_scaleX = 0.7f;
			_scaleY = 0.7f;
			_crashScaleMax = 0.9f;
			_hpMax = _hpMax / 2;
			_hp = _hpMax * 0.85f;
			break;
		case 2:
			_width = 512.f * 0.4f;
			_height = 512.f * 0.4f;
			_scaleX = 0.4f;
			_scaleY = 0.4f;
			_crashScaleMax = 0.6f;
			_hpMax = _hpMax / 2;
			_hp = _hpMax * 0.6f;
			break;
		case 3:
			_width = 512.f * 0.2f;
			_height = 512.f * 0.2f;
			_scaleX = 0.2f;
			_scaleY = 0.2f;
			_crashScaleMax = 0.4f;
			_hpMax = _hpMax / 2;
			_hp = _hpMax * 0.4f;
			break;
		}
		_isBoss2Init = true;
	}
}







void EnemyManager::EnemyUpdata(Enemy::Vector2 playerPos, vector<vector<char>> mapData, float bgHeight, float minMapSize)
{
	//应该要写上状态机变换才对，不应该直接就通过生命值来变换，这样不利于拓展
	//不过为了做的快，其实直接就是两个状态了，一个活着一个死亡
	for (Enemy* element : _enemyUpdateVector) {
		if (element->_hp <= 0) {
			element->ToDead();
		}
		else {
			element->CollideSystem();
			element->Move(playerPos, mapData, bgHeight, minMapSize);
			element->ShootAttack();
			element->DropSystem(mapData, bgHeight, minMapSize);
			element->Effect();
			if (element->_type == Enemy::boss || element->_type == Enemy::boss2) {
				element->EnemyBoss();
			}
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
	case Enemy::dog1_1:
		if (_enemyIdiePool_dog1_1.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_dog1_1.front();
			_enemyIdiePool_dog1_1.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	case Enemy::dog2:
		if (_enemyIdiePool_dog2.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_dog2.front();
			_enemyIdiePool_dog2.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	case Enemy::shoot:
		if (_enemyIdiePool_shoot.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_shoot.front();
			_enemyIdiePool_shoot.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	case Enemy::doubleShoot:
		if (_enemyIdiePool_doubleShoot.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_doubleShoot.front();
			_enemyIdiePool_doubleShoot.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	case Enemy::boss:
		if (_enemyIdiePool_boss.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_boss.front();
			_enemyIdiePool_boss.pop();
			enemy->Inital(type);
			return enemy;
		}
		break;
	case Enemy::boss2:
		if (_enemyIdiePool_boss2.empty()) {
			Enemy* enemy = new Enemy(type);
			return enemy;
		}
		else {
			Enemy* enemy = _enemyIdiePool_boss2.front();
			_enemyIdiePool_boss2.pop();
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
	case Enemy::dog: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_dog.push(enemy);
		break; }
	case Enemy::dog1_1: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_dog1_1.push(enemy);
		break; }
	case Enemy::dog2: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_dog2.push(enemy);
		break; }
	case Enemy::shoot: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_shoot.push(enemy);
		break; }
	case Enemy::doubleShoot: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_doubleShoot.push(enemy);
		break; }
	case Enemy::boss: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_boss.push(enemy);
		break; }
	case Enemy::boss2: {
		auto it = find(_enemyUpdateVector.begin(), _enemyUpdateVector.end(), enemy);
		if (it != _enemyUpdateVector.end()) {
			_enemyUpdateVector.erase(it);
		}
		_enemyIdiePool_boss2.push(enemy);
		break; }
	}
}

void EnemyManager::EnemyBornToMap(vector<vector<char>> mapData, float bgW, float bgH, float minSize)
{
	bgW;
	Enemy::Vector2 minMapPos = { 0,0 };
	minMapPos.x = minSize / 2;
	minMapPos.y = bgH - minSize / 2;
	for (const auto& row : mapData) {
		Enemy* emeny = nullptr;
		for (const char& line : row) {
			switch (line) {
			case 'e':
				emeny = EnemyManager::AcquireEnemy(Enemy::dog);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			case 'f':
				emeny = EnemyManager::AcquireEnemy(Enemy::dog2);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			case 'g':
				emeny = EnemyManager::AcquireEnemy(Enemy::shoot);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			case 'h':
				emeny = EnemyManager::AcquireEnemy(Enemy::dog1_1);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			case 'i':
				emeny = EnemyManager::AcquireEnemy(Enemy::doubleShoot);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			case 'b':
				emeny = EnemyManager::AcquireEnemy(Enemy::boss);
				emeny->Fire(Enemy::Vector2{ minMapPos.x, minMapPos.y });
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}

void EnemyManager::EnemyShow(bool isHpLine)
{
	for (Enemy* element : EnemyManager::_enemyUpdateVector) {
		element->Show(isHpLine);
	}
}
