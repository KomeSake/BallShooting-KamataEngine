#pragma once
#include <vector>
#include <queue>
#include <map>
#include "People.h"
using namespace std;

class Enemy :
	public People
{
public:
	static enum EnemyType
	{
		dog,
		dog1_1,
		dog2,
		shoot,
		doubleShoot,
		boss,
		boss2
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
	float _scaleX;
	float _scaleY;
	unsigned int _color;

	EnemyType _type;
	float _hp;
	float _hpMax;
	float _damage;

	bool _isRunToPlayer;//是否追逐玩家
	int _shootTimer;//远程攻击间隔
	float _toPlayerLengthStop;//距离玩家多远会停下(这个值为0就是近战)

	bool _isWarning;//通过这个值来表示已经警戒，会开始一系列的反应
	float _warningLength;//距离玩家多远才开始产生警戒

	bool _isCrash;//被撞击了，通过这个值来播放动画等
	float _crashScaleSpeed;//被撞击变大速度
	float _crashScaleMax;//被撞击最大大小
	float _crashScaleReset;//被撞击后恢复原本

	float _hitBox_enemy;//调整敌人之间碰撞的大小
	float _bounceValue_bullet;//被子弹弹开的力大小
	float _bounceValue_player;//被玩家弹开的力大小

	bool _isDrop;//正在陨落(通过这个值会让敌人不追玩家)
	bool _isHarmed;//是否受到伤害（为了特效）
	bool _isDead;//已经死亡，玩家就不需要和它发生碰撞了

	int _bossPattern;//Boss状态，0：远程，1：近战，2：叫小弟
	bool _isBossDead;
	bool _isBossCrash;//Boss被撞
	bool _isBossSplit;
	int _bossSplit;//Boss分裂阶段，0：不分裂，1：第一阶段
	bool _isBoss2Init;//小Boss初始化


	Enemy(EnemyType type);
	void Inital(EnemyType type);
	void Move(Vector2 playerPos, vector<vector<char>> mapData, float bgHeight, float minMapSize);
	void Fire(Vector2 bornPos);
	void CollideSystem();
	void ShootAttack();//远程攻击
	void Effect();
	void ToDead();
	void Show(bool isHpLine);
	void DropSystem(vector<vector<char>> mapData, float bgHeight, float minMapSize);//跌落判断
	void EnemyBoss();//Boss专属机制
};

class EnemyManager {

public:
	inline static std::vector<Enemy*> _enemyUpdateVector;
	inline static std::queue<Enemy*> _enemyIdiePool_dog;
	inline static std::queue<Enemy*> _enemyIdiePool_dog1_1;
	inline static std::queue<Enemy*> _enemyIdiePool_dog2;
	inline static std::queue<Enemy*> _enemyIdiePool_shoot;
	inline static std::queue<Enemy*> _enemyIdiePool_doubleShoot;
	inline static std::queue<Enemy*> _enemyIdiePool_boss;
	inline static std::queue<Enemy*> _enemyIdiePool_boss2;

	static void EnemyUpdata(Enemy::Vector2 playerPos, vector<vector<char>> mapData, float bgHeight, float minMapSize);
	static Enemy* AcquireEnemy(Enemy::EnemyType type);
	static void ReleaseEnemy(Enemy* enemy);

	static void EnemyBornToMap(vector<vector<char>>mapData, float bgW, float bgH, float minSize);
	//是否显示头顶的血条
	static void EnemyShow(bool isHpLine);
};

