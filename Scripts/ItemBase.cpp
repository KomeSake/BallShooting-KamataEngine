#include "ItemBase.h"

void ItemBase::CheckCameraValume(Vector2 cameraPos, float screenW, float screenH)
{
	_cameraPos = cameraPos;
	_screenWidth = screenW;
	_screenHeight = screenH;
}

int ItemBase::MyTimers(int milli, int index)
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

//从Camera类偷来的方法

void ItemBase::FrameTexture(float x, float y, LoadRes::Sprite sprite, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - _screenHeight) * -1;
	Novice::DrawSprite((int)(screenPosX - (float)(sprite.w) / 2), (int)(screenPosY - (float)(sprite.h) / 2), sprite.path, 1, 1, 0, color);
}

void ItemBase::FrameTexture(float x, float y, LoadRes::Sprite sprite, float rad, int color)
{
	Vector2 rotated = { (float)sprite.w * -1 / 2,(float)sprite.h / 2 };
	//RotatedPos
	rotated = AditionRule(rotated, -rad);
	//ScreenPos
	rotated = { rotated.x + x ,rotated.y + y };
	rotated = WorldToScreen(rotated, _cameraPos);

	Novice::DrawSprite((int)rotated.x, (int)rotated.y, sprite.path, 1, 1, rad, color);
}

void ItemBase::FrameTexture(float x, float y, map<int, LoadRes::SpriteList> spList, int index, int color)
{
	int arrSprite = spList[index].path;
	int arrW = spList[index].w, arrH = spList[index].h;
	int arrSpriteW = spList[index].listW, arrSpriteH = spList[index].listH;
	int arrX = spList[index].x, arrY = spList[index].y;

	Vector2 pos = WorldToScreen({ x,y }, _cameraPos);
	Novice::DrawSpriteRect((int)(pos.x - (float)(arrW) / 2), (int)(pos.y - (float)(arrH) / 2), arrX, arrY, arrW, arrH, arrSprite, ((float)arrW / (float)arrSpriteW), ((float)arrH / (float)arrSpriteH), 0, color);
}

void ItemBase::FrameTexture(float x, float y, map<int, LoadRes::SpriteList> spList, int index, float rad, int color)
{
	int arrSprite = spList[index].path;
	int arrW = spList[index].w, arrH = spList[index].h;
	int arrSpriteW = spList[index].listW, arrSpriteH = spList[index].listH;
	int arrX = spList[index].x, arrY = spList[index].y;

	Vector2 rotated = { (float)spList[index].w * -1 / 2,(float)spList[index].h / 2 };
	rotated = AditionRule(rotated, -rad);
	rotated = { rotated.x + x ,rotated.y + y };
	rotated = WorldToScreen(rotated, _cameraPos);
	Novice::DrawSpriteRect((int)rotated.x, (int)rotated.y, arrX, arrY, arrW, arrH, arrSprite, ((float)arrW / (float)arrSpriteW), ((float)arrH / (float)arrSpriteH), rad, color);
}

void ItemBase::FrameAnimation(float x, float y, map<int, LoadRes::SpriteList> spList, float rad, int color, int frameTime, int playIndex)
{
	if (FrameTimers(frameTime, playIndex)) {
		_frameAniIndex[playIndex]++;
	}
	if (_frameAniIndex[playIndex] > (int)spList.size() - 1 || _frameAniIndex[playIndex] < 0) {
		_frameAniIndex[playIndex] = 0;
	}
	int arrPath = spList[_frameAniIndex[playIndex]].path;
	int arrW = spList[_frameAniIndex[playIndex]].w, arrH = spList[_frameAniIndex[playIndex]].h;
	int arrX = spList[_frameAniIndex[playIndex]].x, arrY = spList[_frameAniIndex[playIndex]].y;
	int arrListW = spList[_frameAniIndex[playIndex]].listW, arrListH = spList[_frameAniIndex[playIndex]].listH;
	Vector2 rotated = { (float)spList[_frameAniIndex[playIndex]].w * -1 / 2,(float)spList[_frameAniIndex[playIndex]].h / 2 };
	rotated = AditionRule(rotated, -rad);
	rotated = { rotated.x + x ,rotated.y + y };
	rotated = WorldToScreen(rotated, _cameraPos);
	Novice::DrawSpriteRect((int)rotated.x, (int)rotated.y, arrX, arrY, arrW, arrH, arrPath, ((float)arrW / (float)arrListW), ((float)arrH / (float)arrListH), rad, color);
}

int ItemBase::FrameTimers(int milli, int index)
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

ItemBase::Vector2 ItemBase::AditionRule(Vector2 pos, float rad)
{
	float aditionX = pos.x * cosf(rad) - pos.y * sinf(rad);
	float aditionY = pos.y * cosf(rad) + pos.x * sinf(rad);
	return Vector2{ aditionX,aditionY };
}

ItemBase::Vector2 ItemBase::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY) {
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}

ItemBase::Vector2 ItemBase::WorldToScreen(Vector2 worldPos, Vector2 cameraPos)
{
	float screenPosX = worldPos.x - cameraPos.x + 1920.f / 2;
	float screenPosY = (worldPos.y - cameraPos.y + 1080.f / 2 - 1080) * -1;
	return Vector2(screenPosX, screenPosY);
}

float ItemBase::SpriteToObjDir(Vector2 dir)
{

	float rad = atan2f(dir.y, dir.x);
	float degree = 180.f / acosf(-1) * rad;
	if (degree < 0) {
		degree += 360;
	}
	degree -= 90;
	rad = -(acosf(-1) / 180 * degree);
	return rad;
}