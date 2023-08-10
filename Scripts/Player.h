#pragma once
#include "./Scripts/People.h"
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

	Player();
	void Move(char keys[], float bgWidth, float bgHeight, float minMapSize);
};