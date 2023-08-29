#include "GameUI.h"

void GameUI::FrameTexture(float x, float y, int sprite, unsigned int color)
{
	Novice::DrawSprite((int)x, (int)y, sprite, 1, 1, 0, color);
}

void GameUI::FrameTexture(float x, float y, std::map<int, LoadRes::SpriteList> sprite, int index, unsigned int color)
{
	int arrSprite = sprite[index].path;
	int arrW = sprite[index].w, arrH = sprite[index].h;
	int arrX = sprite[index].x, arrY = sprite[index].y;
	int arrSpriteW = sprite[index].listW, arrSpriteH = sprite[index].listH;
	Novice::DrawSpriteRect((int)x, (int)y, arrX, arrY, arrW, arrH, arrSprite, ((float)arrW / (float)arrSpriteW), ((float)arrH / (float)arrSpriteH), 0, color);
}

void GameUI::FrameAnimation(float x, float y, std::map<int, LoadRes::SpriteList> sprite, float angle, int frameTime, int index)
{
	if (FrameTimers(frameTime, index)) {
		_frameAniIndex[index]++;
	}
	if (_frameAniIndex[index] > (int)sprite.size() - 1 || _frameAniIndex[index] < 0) {
		_frameAniIndex[index] = 0;
	}
	int arrSprite = sprite[_frameAniIndex[index]].path;
	int arrW = sprite[_frameAniIndex[index]].w, arrH = sprite[_frameAniIndex[index]].h;
	int arrX = sprite[_frameAniIndex[index]].x, arrY = sprite[_frameAniIndex[index]].y;
	int arrSpriteW = sprite[_frameAniIndex[index]].listW, arrSpriteH = sprite[_frameAniIndex[index]].listH;
	Novice::DrawSpriteRect((int)x, (int)y, arrX, arrY, arrW, arrH, arrSprite, ((float)arrW / (float)arrSpriteW), ((float)arrH / (float)arrSpriteH), angle, WHITE);
}

int GameUI::MyTimers(int milli, int index)
{
	if (!_isTimeOpen[index]) {
		_timeStart[index] = clock();
		_isTimeOpen[index] = true;
	}
	_timeEnd[index] = clock();
	if (_timeEnd[index] - _timeStart[index] > milli) {
		_isTimeOpen[index] = false;
		return 1;
	}
	return 0;
}

int GameUI::FrameTimers(int milli, int index)
{
	if (!_frame_isTimeOpen[index]) {
		_frame_timeStart[index] = clock();
		_frame_isTimeOpen[index] = true;
	}
	_frame_timeEnd[index] = clock();
	if (_frame_timeEnd[index] - _frame_timeStart[index] > milli) {
		_frame_isTimeOpen[index] = false;
		return 1;
	}
	return 0;
}

PlayerUI_HP::PlayerUI_HP()
{
	_width = 700;
	_height = 404;
	_pos = { 8 * 4,1080 - _height - 4 * 4 };
	_color = WHITE;

	_radarPos = { _pos.x + 30 * 4,_pos.y + 70 * 4 };
	_radarLengthMax = 1300;
	_radarRate = 0.07f;
	_radarEnemyTimer = 700;
	_isRadarMy = false;

	_isInitial = false;
	_playerHpMax = 0;
	_playerHpSpriteW = 0;
	_hpSpriteRate = 0;
	_radarSpritePos = _pos;
}

void PlayerUI_HP::UIOpen(Player* obj)
{
	//进行一些初始化操作
	if (!_isInitial) {
		_playerHpMax = obj->_hp;
		_playerHpSpriteW = 113 * 4;
		_hpSpriteRate = _playerHpSpriteW / _playerHpMax;
		_isInitial = true;
	}
	//血量
	FrameTexture(_radarSpritePos.x, _radarSpritePos.y, LoadRes::_spUI_playerHp, 0, _color);
	FrameTexture(_pos.x + 60 * 4, _pos.y + 81 * 4, LoadRes::_spUI_playerHp, 1, _color);
	float playerHp = obj->_hp;
	if (playerHp > 0) {
		Novice::DrawBox(int(_pos.x + 60 * 4), int(_pos.y + 85 * 4), int(playerHp * _hpSpriteRate), 9 * 4, 0, 0xa75f53ff, kFillModeSolid);
	}
	FrameTexture(_pos.x + 64 * 4, _pos.y + 85 * 4, LoadRes::_spUI_playerHp, 3, _color);
	//受伤抖动效果
	if (obj->_isHarmed && !obj->_isDead) {
		float moveSpeedX = 5;
		if (MyTimers(50, 5)) {
			moveSpeedX *= -1;
		}
		_radarSpritePos.x += moveSpeedX;
	}
	else {
		_radarSpritePos = _pos;
	}
	//雷达电台
	//常态化显示敌人版
	//for (Enemy* element : EnemyManager::_enemyUpdateVector) {
	//	Vector2 dir = { element->_pos.x - obj->_pos.x,element->_pos.y - obj->_pos.y };
	//	float length = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
	//	if (length < _radarLengthMax) {
	//		Vector2 radarEnemy = { _radarPos.x + dir.x * _radarRate,_radarPos.y + dir.y * _radarRate * -1 };
	//		Novice::DrawEllipse(int(radarEnemy.x), int(radarEnemy.y), 4, 4, 0, 0xe84e40ff, kFillModeSolid);
	//	}
	//}
	//一段时间更新敌人坐标版
	if (MyTimers(_radarEnemyTimer, 4)) {
		_radarEnemyPos.clear();
		for (Enemy* element : EnemyManager::_enemyUpdateVector) {
			Vector2 dir = { element->_pos.x - obj->_pos.x,element->_pos.y - obj->_pos.y };
			float length = sqrtf(powf(dir.x, 2) + powf(dir.y, 2));
			if (length < _radarLengthMax) {
				Vector2 radarEnemy = { _radarPos.x + dir.x * _radarRate,_radarPos.y + dir.y * _radarRate * -1 };
				_radarEnemyPos.push_back(radarEnemy);
			}
		}
	}
	for (Vector2 it : _radarEnemyPos) {
		Novice::DrawEllipse(int(it.x), int(it.y), 4, 4, 0, 0xe84e40ff, kFillModeSolid);
	}
	if (MyTimers(_radarEnemyTimer, 2)) {
		_isRadarMy = true;
	}
	if (MyTimers(_radarEnemyTimer * 3, 3)) {
		_isRadarMy = false;
	}
	if (_isRadarMy) {
		Novice::DrawEllipse(int(_radarPos.x + 2), int(_radarPos.y + 2), 6, 6, 0, 0xffa000ff, kFillModeSolid);
	}
}

PlayerUI_Gun::PlayerUI_Gun()
{
	_width = 162 * 4;
	_height = 66 * 4;
	_pos = { 1920 - _width - 3 * 4,1080 - _height - 2 * 4 };
	_color = WHITE;

	_gunHotPos = _pos;
}

void PlayerUI_Gun::UIOpen(Player* obj)
{
	FrameTexture(_pos.x, _pos.y, LoadRes::_spUI_playerGun, 0, _color);
	if (!obj->_isGunHot) {
		FrameTexture(_gunHotPos.x + 90 * 4, _gunHotPos.y + 14 * 4, LoadRes::_spUI_playerGun, 1, _color);
	}
	float gunhot = float(obj->_gunHotValue);
	float gunhotSpriteW = 252.f;
	float gunhotRate = gunhotSpriteW / float(obj->_gunHotMax);
	if (gunhot > 0) {
		Novice::DrawQuad(
			int(_gunHotPos.x + 90 * 4), int(_gunHotPos.y + 14 * 4),
			int(_gunHotPos.x + 90 * 4 + gunhot * gunhotRate), int(_gunHotPos.y + 14 * 4),
			int(_gunHotPos.x + 90 * 4), int(_gunHotPos.y + 14 * 4 + LoadRes::_spUI_playerGun02.h),
			int(_gunHotPos.x + 90 * 4 + gunhot * gunhotRate), int(_gunHotPos.y + 14 * 4 + LoadRes::_spUI_playerGun02.h),
			0, 0,
			int(gunhot * gunhotRate), LoadRes::_spUI_playerGun02.h,
			LoadRes::_spUI_playerGun02.path,
			_color);
	}

	//如果在枪冷却途中也按下鼠标左键，就晃动
	if (obj->_isGunHot && Novice::IsPressMouse(0)) {
		float moveSpeedX = 5;
		float moveSpeedY = 5;
		if (MyTimers(50, 6)) {
			moveSpeedX *= -1;
			moveSpeedY *= -1;
		}
		_gunHotPos.x += moveSpeedX;
		_gunHotPos.y -= moveSpeedY;
		if (MyTimers(100, 7)) {
			_gunHotPos = _pos;
		}
	}
	else {
		_gunHotPos = _pos;
	}
}

PlayerUI_Steam::PlayerUI_Steam()
{
	_width = 219 * 4;
	_height = 31 * 4;
	_pos = { 124 * 4,1080 - 42 * 4 };
	_color = WHITE;
	_isSteamLight = false;
}

void PlayerUI_Steam::UIOpen(Player* obj)
{
	FrameTexture(_pos.x, _pos.y, LoadRes::_spUI_playerSteam, 0, _color);
	float steam = float(obj->_steamValue);
	float steamSpriteW = float(LoadRes::_spUI_playerSteam01.w);
	float steamRate = steamSpriteW / float(obj->_steamMax);
	if (steam > 0) {
		Novice::DrawQuad(
			int(_pos.x + 17 * 4), int(_pos.y + 7 * 4),
			int(_pos.x + 17 * 4 + steam * steamRate), int(_pos.y + 7 * 4),
			int(_pos.x + 17 * 4), int(_pos.y + 7 * 4 + LoadRes::_spUI_playerSteam01.h),
			int(_pos.x + 17 * 4 + steam * steamRate), int(_pos.y + 7 * 4 + LoadRes::_spUI_playerSteam01.h),
			0, 0,
			int(steam * steamRate), LoadRes::_spUI_playerSteam01.h,
			LoadRes::_spUI_playerSteam01.path,
			_color);
	}
	FrameTexture(_pos.x + 47 * 4, _pos.y + 4 * 4, LoadRes::_spUI_playerSteam, 2, _color);
	//低于%的情况闪烁红灯
	if (steam * steamRate < steamSpriteW / 100 * 16.5f) {
		if (MyTimers(200, 1)) {
			_isSteamLight = true;
		}
		if (MyTimers(600, 0)) {
			_isSteamLight = false;
		}
	}
	else {
		_isSteamLight = false;
	}
	if (_isSteamLight) {
		FrameTexture(_pos.x, _pos.y, LoadRes::_spUI_playerSteam, 3, _color);
	}
}

ScreenUI_Start::ScreenUI_Start()
{
	_width = 1920;
	_height = 1080;
	_pos = { 0,0 };
	_color = WHITE;

	_isStartButton = false;
	_isLevelButton = false;
}

void ScreenUI_Start::UIOpen(Vector2 mousePos)
{
	FrameTexture(40 * 4, 35 * 4, LoadRes::_spUI_screenStart, 0, _color);
	FrameTexture(232 * 4, 116 * 4, LoadRes::_spUI_screenStart, 1, _color);
	FrameTexture(295 * 4, 166 * 4, LoadRes::_spUI_screenStart, 2, _color);
	FrameTexture(295 * 4, 203 * 4, LoadRes::_spUI_screenStart, 2, _color);
	FrameTexture(317 * 4, 171 * 4, LoadRes::_spUI_screenStart, 4, _color);
	FrameTexture(317 * 4, 208 * 4, LoadRes::_spUI_screenStart, 5, _color);
	if (mousePos.x > 295 * 4 && mousePos.y > 166 * 4 && mousePos.x < 371 * 4 && mousePos.y < 193 * 4) {
		FrameTexture(295 * 4, 166 * 4, LoadRes::_spUI_screenStart, 3, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isStartButton = true;
		}
	}
	if (mousePos.x > 295 * 4 && mousePos.y > 203 * 4 && mousePos.x < 371 * 4 && mousePos.y < 230 * 4) {
		FrameTexture(295 * 4, 203 * 4, LoadRes::_spUI_screenStart, 3, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isLevelButton = true;
		}
	}
}

ScreenUI_LevelClear::ScreenUI_LevelClear()
{
	_width = 1920;
	_height = 1080;
	_pos = { 0,0 };
	_color = WHITE;
	_isLevelNext = false;
	_isBackMenu = false;
}

void ScreenUI_LevelClear::UIOpen(Vector2 mousePos)
{
	Novice::DrawBox(0, 0, 1920, 1080, 0, 0x21212190, kFillModeSolid);
	FrameTexture(_pos.x + 38 * 4, _pos.y + 14 * 4, LoadRes::_spUI_screenLevelClear, 0, _color);
	FrameTexture(_pos.x + 253 * 4, _pos.y + 108 * 4, LoadRes::_spUI_screenLevelClear, 1, _color);
	FrameTexture(_pos.x + 277 * 4, _pos.y + 211 * 4, LoadRes::_spUI_screenLevelClear, 2, _color);
	FrameTexture(_pos.x + 136 * 4, _pos.y + 211 * 4, LoadRes::_spUI_screenLevelClear, 3, _color);
	FrameTexture(_pos.x + 301 * 4, _pos.y + 215 * 4, LoadRes::_spUI_screenLevelClear, 5, _color);
	FrameTexture(_pos.x + 157 * 4, _pos.y + 215 * 4, LoadRes::_spUI_screenLevelClear, 6, _color);
	if (mousePos.x > 277 * 4 && mousePos.y > 211 * 4 && mousePos.x < 353 * 4 && mousePos.y < 238 * 4) {
		FrameTexture(277 * 4, 211 * 4, LoadRes::_spUI_screenLevelClear, 4, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isLevelNext = true;
		}
	}
	if (mousePos.x > 136 * 4 && mousePos.y > 211 * 4 && mousePos.x < 212 * 4 && mousePos.y < 238 * 4) {
		FrameTexture(136 * 4, 211 * 4, LoadRes::_spUI_screenLevelClear, 4, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isBackMenu = true;
		}
	}
}

ScreenUI_Dead::ScreenUI_Dead()
{
	_width = 1920;
	_height = 1080;
	_pos = { 0,0 };
	_color = WHITE;
	_isRestart = false;
	_isBackMenu = false;
}

void ScreenUI_Dead::UIOpen(Vector2 mousePos)
{
	Novice::DrawBox(0, 0, 1920, 1080, 0, 0x832f2690, kFillModeSolid);
	FrameTexture(_pos.x + 56 * 4, _pos.y + 35 * 4, LoadRes::_spUI_screenDead, 0, _color);
	FrameTexture(_pos.x + 245 * 4, _pos.y + 35 * 4, LoadRes::_spUI_screenDead, 1, _color);
	FrameTexture(_pos.x + 277 * 4, _pos.y + 211 * 4, LoadRes::_spUI_screenDead, 2, _color);
	FrameTexture(_pos.x + 136 * 4, _pos.y + 211 * 4, LoadRes::_spUI_screenDead, 3, _color);
	FrameTexture(_pos.x + 157 * 4, _pos.y + 215 * 4, LoadRes::_spUI_screenDead, 5, _color);
	FrameTexture(_pos.x + 292 * 4, _pos.y + 215 * 4, LoadRes::_spUI_screenDead, 6, _color);
	if (mousePos.x > 277 * 4 && mousePos.y > 211 * 4 && mousePos.x < 353 * 4 && mousePos.y < 238 * 4) {
		FrameTexture(277 * 4, 211 * 4, LoadRes::_spUI_screenDead, 4, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isRestart = true;
		}
	}
	if (mousePos.x > 136 * 4 && mousePos.y > 211 * 4 && mousePos.x < 212 * 4 && mousePos.y < 238 * 4) {
		FrameTexture(136 * 4, 211 * 4, LoadRes::_spUI_screenDead, 4, _color);
		if (Novice::IsTriggerMouse(0)) {
			_isBackMenu = true;
		}
	}
}
