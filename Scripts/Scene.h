#pragma once
#include "Player.h"
#include "GameUI.h"
class Scene
{
public:
	inline static enum SceneIndex
	{
		Loading,
		Start,
		Game,
		GameOver,
	}_sceneIndex;

	Scene(Player* playerObj);
	void ScreenGame(Player* playerObj);

	PlayerUI_HP* PlayerUI_HPObj;
	PlayerUI_Gun* PlayerUI_GunObj;
	PlayerUI_Steam* PlayerUI_SteamObj;
};

