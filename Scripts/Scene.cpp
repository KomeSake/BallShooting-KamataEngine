#include "Scene.h"

Scene::Scene()
{
	_sceneIndex = Loading;//初始化等于Loading，就会把全部数据都进行载入
	_levelNum = 101;//初始要等于开始界面(101)的数据

	PlayerUI_HPObj = new PlayerUI_HP();
	PlayerUI_GunObj = new PlayerUI_Gun;
	PlayerUI_SteamObj = new PlayerUI_Steam;
	ScreenUI_StartObj = new ScreenUI_Start;
	ScreenUI_LevelClearObj = new ScreenUI_LevelClear;
	ScreenUI_DearObj = new ScreenUI_Dead;
}

void Scene::ScreenGame(Player* playerObj)
{
	PlayerUI_HPObj->UIOpen(playerObj);
	PlayerUI_GunObj->UIOpen(playerObj);
	PlayerUI_SteamObj->UIOpen(playerObj);
}

void Scene::ScreenLevelClear(int mouseX, int mouseY)
{
	ScreenUI_LevelClearObj->UIOpen(GameUI::Vector2{ float(mouseX),float(mouseY) });
	if (ScreenUI_LevelClearObj->_isLevelNext) {
		_sceneIndex = Loading;
		_levelNum++;
		ScreenUI_LevelClearObj->_isLevelNext = false;
	}
	if (ScreenUI_LevelClearObj->_isBackMenu) {
		_sceneIndex = Loading;
		_levelNum = 101;
		ScreenUI_LevelClearObj->_isBackMenu = false;
	}
}

void Scene::ScreenDead(int mouseX, int mouseY)
{
	ScreenUI_DearObj->UIOpen(GameUI::Vector2{ float(mouseX),float(mouseY) });
	if (ScreenUI_DearObj->_isRestart) {
		_sceneIndex = Loading;
		ScreenUI_DearObj->_isRestart = false;
	}
	if (ScreenUI_DearObj->_isBackMenu) {
		_sceneIndex = Loading;
		_levelNum = 101;
		ScreenUI_DearObj->_isBackMenu = false;
	}
}

void Scene::ScreenStart(int mouseX, int mouseY)
{
	ScreenUI_StartObj->UIOpen(GameUI::Vector2{ float(mouseX),float(mouseY) });
	if (ScreenUI_StartObj->_isStartButton) {
		_sceneIndex = Loading;
		_levelNum = 1;
		ScreenUI_StartObj->_isStartButton = false;
	}
}
