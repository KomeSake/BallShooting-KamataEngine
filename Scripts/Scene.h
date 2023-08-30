#pragma once
#include <fstream>
#include "Player.h"
#include "GameUI.h"
#include "Map.h"


class Scene
{
public:
	inline static enum SceneIndex
	{
		Loading,//用于一次载入的场景 
		Start,
		Game,
		GameOver,
		LevelClear,
	}_sceneIndex;

	//没有时间弄Level类了，直接通过以下值来控制每关的地图数据即可做到换关的效果
	inline static enum LevelMapData
	{
		Help,
		Map1,
		Map2,
		Map3,
		Map4,
	}_levelMapData;
	int _levelNum;//用于控制要读取什么地图数据，通过上面的枚举可以获得不同的地图数据
	int _levelLockNum;//已解锁多少关卡
	int _levelMax;//总关卡数(不包含教学关)
	bool _isStartMapData;//是否读取主菜单的地图数据
	Scene();
	void ScreenGame(Player* playerObj);
	//现在因为会让关卡等于101来变成开始菜单地图，所以每次点Start按钮都是从第0关开始，需要更改
	void ScreenStart(int mouseX, int mouseY);
	bool _isExitButton;//开始界面有一个退出按钮
	void ScreenLevelClear(int mouseX, int mouseY);
	void ScreenDead(int mouseX, int mouseY);
	//存档机制，0：读取，1：写入
	void LocalSave(int index);

	PlayerUI_HP* PlayerUI_HPObj;
	PlayerUI_Gun* PlayerUI_GunObj;
	PlayerUI_Steam* PlayerUI_SteamObj;

	ScreenUI_Start* ScreenUI_StartObj;
	ScreenUI_LevelClear* ScreenUI_LevelClearObj;
	ScreenUI_Dead* ScreenUI_DearObj;
};

