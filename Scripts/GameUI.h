#pragma once
#include <Novice.h>
#include <map>
#include <ctime>
#include "LoadRes.h"
#include "Player.h"
using namespace std;

//所有和UI相关的类都会写在这里面
class GameUI
{
public:
	struct Vector2
	{
		float x;
		float y;
	};

	Vector2 _pos;
	float _width, _height;
	unsigned int _color;
	//UI的图像渲染均是左上角为开始点，并且是以屏幕坐标
	void FrameTexture(float x, float y, int sprite, unsigned int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList> sprite, int index, unsigned int color);
	void FrameAnimation(float x, float y, map<int, LoadRes::SpriteList> sprite, float angle, int frameTime, int index);

	//计时器(共30格)
	int MyTimers(int milli, int index);
	clock_t _timeStart[31] = { 0 };
	clock_t _timeEnd[31] = { 0 };
	bool _isTimeOpen[31] = { 0 };
	//帧动画计时器
	int FrameTimers(int milli, int index);
	int _frameAniIndex[31] = { 0 };//从指定帧开始播放(例如立马回到0帧)
	clock_t _frame_timeStart[31] = { 0 };
	clock_t _frame_timeEnd[31] = { 0 };
	bool _frame_isTimeOpen[31] = { 0 };
};

class PlayerUI_HP :
	public GameUI
{
public:
	float _playerHpMax;//记录玩家的血上限
	float _playerHpSpriteW;//显示血量红色长条的长
	float _hpSpriteRate;//计算出两者的比率，用来控制血条UI长度
	PlayerUI_HP(Player* obj);
	void UIOpen(Player* obj);
};