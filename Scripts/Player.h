#pragma once
#include "People.h"

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

	int _pattern;//状态指示器，0球，1人，2蒸汽冲刺(未做)
	//蒸汽冲刺想法：把Move方法复制一份，把里面的碰撞检测和物理移动挑出即可。
	//最后在方法中设定一个时间来退出，回到普通的移动模式

	LoadRes::Sprite _sprite;
	float _width;
	float _height;
	unsigned int _color;

	Vector2 _mousePos;
	Vector2 _bulletPos;
	Vector2 _bulletDir;
	int _bulletTime;//子弹间隔时间

	Player();
	void Move(char keys[], float bgWidth, float bgHeight, float minMapSize);
	void Attack(Vector2 cameraPos);
	void PatternChange(char keys[], char preKeys[]);
};