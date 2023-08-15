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

	LoadRes::Sprite _sprite;
	float _width;
	float _height;
	unsigned int _color;

	Vector2 _mousePos;
	Vector2 _bulletPos;
	Vector2 _bulletDir;
	float _bulletSpeed;
	int _bulletTime;//子弹间隔时间

	Player();
	void Move(char keys[], float bgWidth, float bgHeight, float minMapSize);
	void Attack(Vector2 cameraPos);
};