#pragma once
#include "People.h"
#include "Enemy.h"
#include <map>

class Player :
	public People
{
public:
	Vector2 _pos;//postion
	Vector2 _dir;//direction
	Vector2 _vel;//velocity
	Vector2 _acceleration;
	float _speed;
	float _bounce;//反弹系数
	float _friction;//摩擦力
	float _velMax;//速度的上限，正反皆是
	float _speedBall, _bounceBall, _frictionBall, _velMaxBall;
	float _speedMan, _bounceMan, _frictionMan, _velMaxMan;

	//蒸汽冲刺想法：把Move方法复制一份，把里面的碰撞检测和物理移动挑出即可。
	//最后在方法中设定一个时间来退出，回到普通的移动模式

	int _pattern;//状态指示器，0球，1人，2蒸汽不足
	bool _isBallEntering;//正在变成球形态
	bool _isManEntering;//正在变成人形态
	float _hp;
	bool _isHpMinus;//判断是否被扣血了，扣血了就触发无敌时间
	int _hpGodTime;//扣血无敌时间
	float _dropDamage;//跌落伤害
	float _bounceValue_enemy;//被敌人触碰的反弹力
	float _ballDamage;
	bool _isBallTouch;//球形态下是否撞中敌人(用于给Camera类指示)
	bool _isHarmed;

	map<int, LoadRes::SpriteList> _sprite;
	float _width;
	float _height;
	float _scaleX;
	float _scaleY;
	unsigned int _color;

	Vector2 _mousePos;
	Vector2 _bulletPos;
	Vector2 _bulletDir;
	int _bulletTime;//子弹间隔时间

	int _steamValue;//蒸汽当前数量
	int _steamMax;//蒸汽上限
	int _steamEnemy;//撞击敌人后减少多少蒸汽
	float _steamPlusRate;//增加增加比率
	int _steamMinus;
	int _gunHotPlus;//武器增加热度
	int _gunHotMinus;//武器减少热度
	int _gunHotValue;//当前热度容量
	int _gunHotMax;//热度上限
	bool _isGunHot;

	Player();
	void Move(char keys[], vector<vector<char>> mapData, float bgWidth, float bgHeight, float minMapSize);
	void Attack(Vector2 cameraPos);
	void SteamPush();//蒸汽系统
	void GunHot();//武器过热系统
	void PatternChange(char keys[], char preKeys[]);
	void CollideSystem();
	void IsDead();
	void DropSystem(vector<vector<char>> mapData, float bgHeight, float minMapSize);
	bool _isDrop;//玩家在掉落的时候就不能移动和攻击了
	Vector2 _dropPos[3];//记录下3个生前(跌落前)的坐标
	void Show();
	float _spriteDownDegree;//地盘动画的角度
	int _ballStopSprite;//球停下来的时候需要用什么图片
	bool _isBallStop;//球形态初次停下来了
	void Effect();
};