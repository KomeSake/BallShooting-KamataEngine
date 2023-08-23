#pragma once
#include <vector>
#include <queue>
#include <map>
#include "People.h"
#include "Bullet.h"
using namespace std;

class Enemy :
	public People
{
public:
	static enum EnemyType
	{
		dog,
		dog2,
	}enumEnemy;

	Vector2 _pos;
	Vector2 _dir;
	Vector2 _vel;//velocity
	Vector2 _acceleration;
	float _speed;
	float _bounce;//反弹系数
	float _friction;//摩擦力
	float _velMax;//速度的上限，正反皆是

	map<int, LoadRes::SpriteList> _sprite;
	float _width;
	float _height;
	unsigned int _color;

	EnemyType _type;
	float _hp;
	float _damage;
	float _warningLength;//距离玩家多远才开始产生警戒
	bool _isWarning;//通过这个值来表示已经警戒，会开始一系列的反应
	float _hitBox_enemy;//调整敌人之间碰撞的大小
	float _bounceValue_bullet;//被子弹弹开的力大小
	float _bounceValue_player;//被玩家弹开的力大小

	bool _isHarmed;//是否受到伤害（为了特效）

	Enemy(EnemyType type);
	void Inital(EnemyType type);
	void Move(Vector2 playerPos);
	void Fire(Vector2 bornPos);
	void CollideSystem(Vector2 playerPos, int playerPattern, float playerBallDamage);
	void Effect();
	void IsDead();
	void Show();
};

class EnemyManager {

public:
	inline static std::vector<Enemy*> _enemyUpdateVector;
	inline static std::queue<Enemy*> _enemyIdiePool_dog;
	inline static std::queue<Enemy*> _enemyIdiePool_dog2;

	static void EnemyUpdata(Enemy::Vector2 playerPos, int playerPattern, float playerBallDamage);
	static Enemy* AcquireEnemy(Enemy::EnemyType type);
	static void ReleaseEnemy(Enemy* enemy);

	static void EnemyBornToMap(vector<vector<char>>mapData, float bgW, float bgH, float minSize);
	static void EnemyShow();
};

