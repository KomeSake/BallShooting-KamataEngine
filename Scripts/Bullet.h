﻿#pragma once
#include <vector>
#include <queue>
#include "ItemBase.h"
using namespace std;

class Bullet :
	public ItemBase
{
public:
	static enum BulletType
	{
		normal,
		enemy_shoot,
		player_shoot,//反弹敌人的子弹
		boss,
		boss_player
	}enumBullet;

	Vector2 _pos;
	Vector2 _dir;
	float _speed;

	LoadRes::Sprite _sprite;
	map<int, LoadRes::SpriteList> _spriteAni;
	bool _isSpriteAni;//子弹是否是动画
	float _width;
	float _height;
	unsigned int _color;

	BulletType _type;
	bool _isAlive;//用来判断此子弹是否还生效(不生效就播放特效了)
	float _damage;

	Bullet(BulletType type);
	void Inital(BulletType type);
	void Move(Vector2 cameraPos, vector<vector<char>> mapData, float bgHeight, float minMapSize);
	void Show();
	void Fire(Vector2 bornPos, Vector2 dir);
	void ToDead();//里面是一些死亡时候要进行的事情(例如播放特效)
	bool _toDeadEffect;//用来初次进行死亡特效时候做的工作
};

class BulletManager {
public:
	inline static vector<Bullet*> _bulletUpdata_player;
	inline static vector<Bullet*> _bulletUpdata_enemy;

	inline static queue<Bullet*> _bulletIdiePool_normal;
	inline static queue<Bullet*> _bulletIdiePool_enemy_shoot;
	inline static queue<Bullet*> _bulletIdiePool_player_shoot;
	inline static queue<Bullet*> _bulletIdiePool_boss;
	inline static queue<Bullet*> _bulletIdiePool_boss_player;

	static void BulletUpdata(Bullet::Vector2 cameraPos, vector<vector<char>> mapData, float bgHeight, float minMapSize);
	static Bullet* AcquireBullet(Bullet::BulletType type);
	static void ReleaseBullet(Bullet* bullet);

	static void BulletShow();
};