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

PlayerUI_HP::PlayerUI_HP(Player* obj)
{
	_width = 700;
	_height = 404;
	_pos = { 8 * 4,1080 - _height - 4 * 4 };
	_color = WHITE;

	_playerHpMax = obj->_hp;
	_playerHpSpriteW = 113 * 4;
	_hpSpriteRate = _playerHpSpriteW / _playerHpMax;
}

void PlayerUI_HP::UIOpen(Player* obj)
{
	FrameTexture(_pos.x, _pos.y, LoadRes::_spUI_playerHp, 0, _color);
	FrameTexture(_pos.x + 60 * 4, _pos.y + 81 * 4, LoadRes::_spUI_playerHp, 1, _color);
	float playerHp = obj->_hp;
	if (playerHp > 0) {
		Novice::DrawBox(int(_pos.x + 60 * 4), int(_pos.y + 85 * 4), int(playerHp * _hpSpriteRate), 9 * 4, 0, 0xa75f53ff, kFillModeSolid);
	}
	FrameTexture(_pos.x + 64 * 4, _pos.y + 85 * 4, LoadRes::_spUI_playerHp, 3, _color);
}

PlayerUI_Gun::PlayerUI_Gun()
{
	_width = 162 * 4;
	_height = 66 * 4;
	_pos = { 1920 - _width - 3 * 4,1080 - _height - 2 * 4 };
	_color = WHITE;
}

void PlayerUI_Gun::UIOpen(Player* obj)
{
	FrameTexture(_pos.x, _pos.y, LoadRes::_spUI_playerGun, 0, _color);
	FrameTexture(_pos.x + 90 * 4, _pos.y + 14 * 4, LoadRes::_spUI_playerGun, 1, _color);
	float gunhot = float(obj->_gunHotValue);
	float gunhotSpriteW = 252.f;
	float gunhotRate = gunhotSpriteW / float(obj->_gunHotMax);
	if (gunhot > 0) {
		Novice::DrawQuad(
			int(_pos.x + 90 * 4), int(_pos.y + 14 * 4),
			int(_pos.x + 90 * 4 + gunhot * gunhotRate), int(_pos.y + 14 * 4),
			int(_pos.x + 90 * 4), int(_pos.y + 14 * 4 + LoadRes::_spUI_playerGun02.h),
			int(_pos.x + 90 * 4 + gunhot * gunhotRate), int(_pos.y + 14 * 4 + LoadRes::_spUI_playerGun02.h),
			0, 0,
			int(gunhot * gunhotRate), LoadRes::_spUI_playerGun02.h,
			LoadRes::_spUI_playerGun02.path,
			_color);
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
