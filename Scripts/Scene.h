#pragma once
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
	}_sceneIndex;

	//没有时间弄Level类了，直接通过以下值来控制每关的地图数据即可做到换关的效果
	inline static enum LevelMapData
	{
		Help,
		Map1,
		Map2,
		Map3,
		Map4,
		Map5,
		StartScreen = 101,
		GameOverScreen,
	}_levelMapData;
	int _levelNum;//用于控制要读取什么地图数据，通过上面的枚举可以获得不同的地图数据
	Scene();
	void ScreenGame(Player* playerObj);
	void ScreenStart(int mouseX,int mouseY);

	PlayerUI_HP* PlayerUI_HPObj;
	PlayerUI_Gun* PlayerUI_GunObj;
	PlayerUI_Steam* PlayerUI_SteamObj;

	ScreenUI_Start* ScreenUI_StartObj;
};

