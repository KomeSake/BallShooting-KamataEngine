#pragma once
#include <Novice.h>
#include <cmath>
#include <ctime>
#include <random>
#include "LoadRes.h"
#include "Map.h"
#include "Bullet.h"
using namespace std;

class People
{
public:
	struct Vector2
	{
		float x;
		float y;
	};

	//用以获取Camera的参数，提供给从Camera类偷来的渲染函数
	static void CheckCameraValume(Vector2 cameraPos, float screenW, float screenH);
	inline static Vector2 _cameraPos;
	inline static float _screenWidth;
	inline static float _screenHeight;

	//计时器(共30格)
	int MyTimers(int milli, int index);
	clock_t _timeStart[31] = { 0 };
	clock_t _timeEnd[31] = { 0 };
	bool _isTimeOpen[31] = { 0 };

	//向量归一化
	Vector2 VectorNormalization(float x, float y);

	//从Camera类偷来的一些方法，帧动画渲染方法
	//(但是这里的更多功能，例如缩放，但是缩放还是不太对劲，需要修改，不过暂时够用了)
	//工具组
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, int color);
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, float rad, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, float rad, int color);
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, float scaleX, float scaleY, int color);
	void FrameTexture_OLD(float x, float y, LoadRes::Sprite sprite, float scaleX, float scaleY, int color);
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, float scaleX, float scaleY, float rad, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, float scaleX, float scaleY, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, float scaleX, float scaleY, float rad, int color);

	//播放帧动画：位置x，位置y，播放文件，角度，每帧时间，几号位置
	void FrameAnimation(float x, float y, map<int, LoadRes::SpriteList>spList, float rad, int color, int frameTime, int playIndex);
	void FrameAnimation(float x, float y, map<int, LoadRes::SpriteList>spList, float scaleX, float scaleY, float rad, int color, int frameTime, int playIndex);

	//将屏幕坐标变成世界坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY);
	//将世界坐标变成屏幕坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 WorldToScreen(Vector2 worldPos, Vector2 cameraPos);
	//让图片旋转成和方向一致
	static float SpriteToObjDir(Vector2 dir);

	//加法定理，为了计算旋转后的图像位置
	Vector2 AditionRule(Vector2 pos, float rad);

	//帧动画所需变量&工具
	static const int _frame_sum = 31;//准备多少个帧动画播放空间
	int _frameAniIndex[_frame_sum] = { 0 };//从指定帧开始播放(例如立马回到0帧)
	int FrameTimers(int milli, int index);//帧动画计时器
	clock_t _frame_timeStart[_frame_sum] = { 0 };
	clock_t _frame_timeEnd[_frame_sum] = { 0 };
	bool _frame_isTimeOpen[_frame_sum] = { 0 };

};

