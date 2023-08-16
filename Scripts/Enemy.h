#pragma once
#include <vector>
#include <queue>
#include "People.h"
using namespace std;

class Enemy :
	public People
{
public:
	static enum EnemyType
	{
		dog,
	}enumEnemy;

	Vector2 _pos;
	Vector2 _dir;
	float _speed;

	LoadRes::Sprite _sprite;
	float _width;
	float _height;
	unsigned int _color;

	EnemyType _type;
	float _damage;
	float _warningLength;//距离玩家多远才开始产生反应

	Enemy(EnemyType type);
	void Inital(EnemyType type);
	void Move(Vector2 playerPos);
	void Fire(Vector2 bornPos);
};

class EnemyManager {

public:
	inline static std::vector<Enemy*> _enemyUpdateVector;
	inline static std::queue<Enemy*> _enemyIdiePool_dog;

	static void EnemyUpdata(Enemy::Vector2 playerPos);
	static Enemy* AcquireEnemy(Enemy::EnemyType type);
	static void ReleaseEnemy(Enemy* enemy);

	static void EnemyBornToMap(vector<vector<char>>mapData, float bgW, float bgH, float minSize);
};

