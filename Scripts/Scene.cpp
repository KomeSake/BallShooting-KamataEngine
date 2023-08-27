#include "Scene.h"

Scene::Scene(Player* playerObj)
{
	_sceneIndex = Game;

	PlayerUI_HPObj = new PlayerUI_HP(playerObj);
	PlayerUI_GunObj = new PlayerUI_Gun;
	PlayerUI_SteamObj = new PlayerUI_Steam;
}

void Scene::ScreenGame(Player* playerObj)
{
	PlayerUI_HPObj->UIOpen(playerObj);
	PlayerUI_GunObj->UIOpen(playerObj);
	PlayerUI_SteamObj->UIOpen(playerObj);
}
