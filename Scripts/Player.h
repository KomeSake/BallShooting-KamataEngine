#pragma once
#include "People.h"

class Player :
	public People
{
public:
	Vector2 _pos;//postion
	Vector2 _dir;//direction
	Vector2 _vel;//velocity
	float _speed;
	unsigned int _image;
	float _width;
	float _height;
	unsigned int _color;

	Vector2 _mousePos;
	Vector2 _bulletPos;
	Vector2 _bulletDir;
	float _bulletSpeed;

	Player();
	void Move(char keys[], float bgWidth, float bgHeight, float minMapSize);
	void Attack(Vector2 cameraPos);
};