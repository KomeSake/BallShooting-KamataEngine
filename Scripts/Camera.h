#pragma once
#include <vector>
#include <cmath>
#include <map>
#include <ctime>
#include "LoadRes.h"
using namespace std;

class Camera
{
public:
	struct Vector2
	{
		float x;
		float y;
	};

	Vector2 _cameraPos;
	Vector2 _cameraOffset;//相机偏移量，用以给相机做位移效果
	Vector2 _bgPos;

	float _bgWidth;
	float _bgHeight;
	float _screenWidth;
	float _screenHeight;
	float _minMapSize;

	Camera(const int screenW, const int screenH, int bgW, int bgH, int minMapSize);
	void Move(Vector2 playerPos);
	void MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize, bool playerIsExit);
	void MouseShow(float mouseX, float mouseY,bool isPlay);
	bool _isPlayMouseShow;//是否播放鼠标特效

	bool CameraEffect(int index);
	int _cameraEffect01[4];//屏幕抖动效果，0：x移动次数，1：y移动次数，2：总共几个轮回，3：移动的速度

	bool _isExitAniOver;//用于结束大门的播放

	//相机设置步骤
	//1.所有坐标都必须是世界坐标
	//2.设置一个相机的坐标
	//3.将相机框选出的部分转换成相机坐标系
	//4.将框选出的部分转换成屏幕坐标系


	//工具组
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, int color);
	void FrameTexture(float x, float y, LoadRes::Sprite sprite, float rad, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, int color);
	void FrameTexture(float x, float y, map<int, LoadRes::SpriteList>spList, int index, float rad, int color);

	//播放帧动画：位置x，位置y，播放文件，角度，每帧时间，几号位置
	void FrameAnimation(float x, float y, map<int, LoadRes::SpriteList>spList, float rad, int color, int frameTime, int playIndex);


	//将屏幕坐标变成世界坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY);
	//将世界坐标变成屏幕坐标(为了方便，屏幕大小已定为1920*1080)
	static Vector2 WorldToScreen(Vector2 worldPos, Vector2 cameraPos);
	//让图片旋转成和方向一致
	static float SpriteToObjDir(Vector2 dir);


	//帧动画所需变量&工具
	static const int _frame_sum = 31;//准备多少个帧动画播放空间
	int _frameAniIndex[_frame_sum] = { 0 };//从指定帧开始播放(例如立马回到0帧)
	int FrameTimers(int milli, int index);//帧动画计时器
	clock_t _frame_timeStart[_frame_sum] = { 0 };
	clock_t _frame_timeEnd[_frame_sum] = { 0 };
	bool _frame_isTimeOpen[_frame_sum] = { 0 };

	//加法定理，为了计算旋转后的图像位置
	Vector2 AditionRule(Vector2 pos, float rad);

	//计时器(共30格)
	int MyTimers(int milli, int index);
	clock_t _timeStart[31] = { 0 };
	clock_t _timeEnd[31] = { 0 };
	bool _isTimeOpen[31] = { 0 };
};