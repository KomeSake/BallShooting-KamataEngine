#pragma once
#include <Novice.h>
#include <vector>
#include <queue>
#include "LoadRes.h"
using namespace std;

class Bullet
{
public:
	struct Vector2
	{
		float x;
		float y;
	};
	static enum BulletType
	{
		normal,
	}bulletType;

	Vector2 _pos;
	Vector2 _dir;
	float _speed;
	BulletType _type;
	unsigned int _image;
	float _width;
	float _height;
	unsigned int _color;

	Bullet(BulletType type);
	void Inital(BulletType type);
	void Move(Vector2 cameraPos);
	void Fire(Vector2 pos, Vector2 dir);
};

class BulletManager {
public:
	inline static vector<Bullet*> _bulletUpdata_player;
	inline static queue<Bullet*> _bulletIdiePool_normal;

	static void BulletUpdata(Bullet::Vector2 cameraPos);
	static Bullet* AcquireBullet(Bullet::BulletType type);
	static void ReleaseBullet(Bullet* bullet);
};