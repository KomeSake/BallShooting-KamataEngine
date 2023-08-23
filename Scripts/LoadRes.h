#pragma once
#include <Novice.h>
#include <map>
using namespace std;

class LoadRes
{
public:
	//普通图片的结构体
	static struct Sprite
	{
		int path = 0;
		int w = 0;
		int h = 0;
	}structSP;
	//这是用来装图集的结构体
	static struct SpriteList
	{
		int path = 0;//本地路径
		int x = 0;//图片在图集中的位置（左上角）
		int y = 0;
		int w = 0;//图片的宽高
		int h = 0;
		int listW = 0;//图集的宽高
		int listH = 0;
	}structSL;

	inline static Sprite _spBullet;
	inline static Sprite _spArrow;

	inline static map<int, SpriteList> _spListPlayer;
	inline static map<int, SpriteList> _spListPlayer_gun;
	inline static map<int, SpriteList> _spListPlayer_down;
	inline static map<int, SpriteList> _spListPlayer_ball;
	inline static map<int, SpriteList> _spListPlayer_smoker;
	inline static map<int, SpriteList> _spListPlayer_ChangeAni;
	inline static map<int, SpriteList> _spListPlayer_ChangeAniTwo;
	inline static map<int, SpriteList> _spListEnemy1;
	inline static map<int, SpriteList> _spListEnemy2;

	inline static map<int, SpriteList> _spListMap;

	inline static map<int, SpriteList> _spListExplode;
	inline static map<int, SpriteList> _spListEnemyExplode;

	static void LoadResNovice();
};

