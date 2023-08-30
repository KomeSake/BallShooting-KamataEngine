#include "Scene.h"

Scene::Scene()
{
	_sceneIndex = Loading;//初始化等于Loading，就会把全部数据都进行载入
	_levelNum = 0;
	_levelLockNum = 0;
	_isStartMapData = true;//游戏最开始要等于true，这样才会载入开始界面的地图数据

	PlayerUI_HPObj = new PlayerUI_HP();
	PlayerUI_GunObj = new PlayerUI_Gun;
	PlayerUI_SteamObj = new PlayerUI_Steam;
	ScreenUI_StartObj = new ScreenUI_Start;
	_isExitButton = false;
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
	if (_levelLockNum < 4 && _levelNum == _levelLockNum) {
		_levelLockNum++;
	}
	ScreenUI_LevelClearObj->UIOpen(GameUI::Vector2{ float(mouseX),float(mouseY) }, _levelNum, _levelLockNum);
	if (ScreenUI_LevelClearObj->_isLevelNext) {
		_sceneIndex = Loading;
		if (_levelNum < 4) {
			_levelNum++;
		}
		ScreenUI_LevelClearObj->_isLevelNext = false;
		ScreenUI_LevelClearObj->_allLevelScale = 2;
	}
	if (ScreenUI_LevelClearObj->_isBackMenu) {
		_sceneIndex = Loading;
		_isStartMapData = true;
		ScreenUI_LevelClearObj->_isBackMenu = false;
		ScreenUI_LevelClearObj->_allLevelScale = 2;
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
		_isStartMapData = true;
		ScreenUI_DearObj->_isBackMenu = false;
	}
}

void Scene::LocalSave(int index)
{
	switch (index) {
	case 0: {
		ifstream file("./Resources/Save/save000.txt");
		if (!file.is_open()) {
			//如果没有这个文件就新建一个
			ofstream newFile("./Resources/Save/save000.txt");
			newFile << _levelLockNum;
			newFile.close();
		}
		else {
			file >> _levelLockNum;
			file.close();
		}
		break; }
	case 1: {
		ofstream file("./Resources/Save/save000.txt");
		if (!file.is_open()) {
			Novice::ConsolePrintf("File load is erro!!\n");
		}
		file << _levelLockNum;
		file.close();
		break; }
	}
}

void Scene::ScreenStart(int mouseX, int mouseY)
{
	ScreenUI_StartObj->UIOpen(GameUI::Vector2{ float(mouseX),float(mouseY) }, _levelNum, _levelLockNum);
	if (ScreenUI_StartObj->_isStartButton) {
		_sceneIndex = Loading;
		ScreenUI_StartObj->_isStartButton = false;
	}
	if (ScreenUI_StartObj->_isExitButton) {
		_isExitButton = true;
	}
}
