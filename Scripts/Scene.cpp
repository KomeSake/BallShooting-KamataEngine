#include "Scene.h"

Scene::Scene()
{
	_sceneIndex = Loading;
	_levelNum = 1;

	PlayerUI_HPObj = new PlayerUI_HP();
	PlayerUI_GunObj = new PlayerUI_Gun;
	PlayerUI_SteamObj = new PlayerUI_Steam;
}

void Scene::ScreenGame(Player* playerObj)
{
	PlayerUI_HPObj->UIOpen(playerObj);
	PlayerUI_GunObj->UIOpen(playerObj);
	PlayerUI_SteamObj->UIOpen(playerObj);
}
