#include "Camera.h"

Camera::Camera(const int screenW, const int screenH, int bgW, int bgH, int minMapSize)
{
	_bgWidth = (float)bgW;
	_bgHeight = (float)bgH;
	_screenWidth = (float)screenW;
	_screenHeight = (float)screenH;
	_minMapSize = (float)minMapSize;

	_cameraPos = { _screenWidth / 2,_screenHeight / 2 };
	_cameraOffset = { 0,0 };
	_bgPos = { _bgWidth / 2,_bgHeight / 2 };

	_cameraEffect01[0] = 0;
	_cameraEffect01[1] = 5;
	_cameraEffect01[2] = 2;
	_cameraEffect01[3] = 5;
}

void Camera::Move(Vector2 playerPos)
{
	if (playerPos.x >= _screenWidth / 2 && playerPos.x <= _bgWidth - _screenWidth / 2) {
		_cameraPos.x = playerPos.x + _cameraOffset.x;
	}
	if (playerPos.y >= _screenHeight / 2 && playerPos.y <= _bgHeight - _screenHeight / 2) {
		_cameraPos.y = playerPos.y + _cameraOffset.y;
	}
}

void Camera::MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize) {
	bgW;
	Vector2 minMapPos = { 0,0 };
	//最下层的地图展示
	minMapPos.x = minSize / 2;
	minMapPos.y = bgH - minSize / 2;
	for (const auto& row : mapData) {
		for (const char& line : row) {
			switch (line) {
			case 'w':
				FrameTexture(minMapPos.x, minMapPos.y, LoadRes::_spListMap, 2, WHITE);
				break;
			case 'o':
				FrameTexture(minMapPos.x, minMapPos.y, LoadRes::_spListMap, 0, WHITE);
				break;
			case 'e':
			case 'f':
				FrameTexture(minMapPos.x, minMapPos.y, LoadRes::_spListMap, 0, WHITE);
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
	//在地图上面的图片(例如提示，地图装饰等。不会遮挡玩家)
	minMapPos.x = minSize / 2;
	minMapPos.y = bgH - minSize / 2;
	for (const auto& row : mapData) {
		for (const char& line : row) {
			switch (line) {
			case 'U':
				FrameTexture(minMapPos.x, minMapPos.y, LoadRes::_spStart, WHITE);
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}

bool Camera::CameraEffect(int index)
{
	switch (index) {
	case 0: {
		if (_cameraEffect01[0] > 0) {
			_cameraOffset.x += _cameraEffect01[3];
			_cameraEffect01[0]--;
		}
		if (_cameraEffect01[1] > 0) {
			_cameraOffset.y += _cameraEffect01[3];
			_cameraEffect01[1]--;
		}
		if (_cameraEffect01[0] <= 0 && _cameraEffect01[1] <= 0) {
			_cameraEffect01[3] = _cameraEffect01[3] * -1;
			_cameraEffect01[0] = 5;
			_cameraEffect01[1] = 5;
			_cameraEffect01[2]--;
		}
		if (_cameraEffect01[2] < 0) {
			_cameraEffect01[0] = 0;
			_cameraEffect01[1] = 5;
			_cameraEffect01[2] = 2;
			_cameraEffect01[3] = 5;
			_cameraOffset = { 0,0 };
			return true;
		}
		break; }
	}
	return false;
}

void Camera::FrameTexture(float x, float y, LoadRes::Sprite sprite, int color)
{
	float screenPosX = x - _cameraPos.x + _screenWidth / 2;
	float screenPosY = (y - _cameraPos.y + _screenHeight / 2 - _screenHeight) * -1;
	Novice::DrawSprite((int)(screenPosX - (float)(sprite.w) / 2), (int)(screenPosY - (float)(sprite.h) / 2), sprite.path, 1, 1, 0, color);
}

void Camera::FrameTexture(float x, float y, LoadRes::Sprite sprite, float rad, int color)
{
	Vector2 rotated = { (float)sprite.w * -1 / 2,(float)sprite.h / 2 };
	//RotatedPos
	rotated = AditionRule(rotated, -rad);
	//ScreenPos
	rotated = { rotated.x + x ,rotated.y + y };
	rotated = WorldToScreen(rotated, _cameraPos);

	Novice::DrawSprite((int)rotated.x, (int)rotated.y, sprite.path, 1, 1, rad, color);
}

void Camera::FrameTexture(float x, float y, map<int, LoadRes::SpriteList> spList, int index, int color)
{
	int arrSprite = spList[index].path;
	int arrW = spList[index].w, arrH = spList[index].h;
	int arrSpriteW = spList[index].listW, arrSpriteH = spList[index].listH;
	int arrX = spList[index].x, arrY = spList[index].y;

	Vector2 pos = WorldToScreen({ x,y }, _cameraPos);
	Novice::DrawSpriteRect((int)(pos.x - (float)(arrW) / 2), (int)(pos.y - (float)(arrH) / 2), arrX, arrY, arrW, arrH, arrSprite, ((float)arrW / (float)arrSpriteW), ((float)arrH / (float)arrSpriteH), 0, color);
}

void Camera::FrameTexture(float x, float y, map<int, LoadRes::SpriteList> spList, int index, float rad, int color)
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

void Camera::FrameAnimation(float x, float y, map<int, LoadRes::SpriteList> spList, float rad, int color, int frameTime, int playIndex)
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

int Camera::FrameTimers(int milli, int index)
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

Camera::Vector2 Camera::AditionRule(Vector2 pos, float rad)
{
	float aditionX = pos.x * cosf(rad) - pos.y * sinf(rad);
	float aditionY = pos.y * cosf(rad) + pos.x * sinf(rad);
	return Vector2{ aditionX,aditionY };
}

Camera::Vector2 Camera::ScreenToWorld(float screenX, float screenY, float cameraX, float cameraY) {
	float worldX = (float)(screenX + cameraX - 1920.f / 2);
	float worldY = (float)(screenY * -1 + cameraY - 1080.f / 2 + 1080);
	return Vector2{ worldX,worldY };
}

Camera::Vector2 Camera::WorldToScreen(Vector2 worldPos, Vector2 cameraPos)
{
	float screenPosX = worldPos.x - cameraPos.x + 1920.f / 2;
	float screenPosY = (worldPos.y - cameraPos.y + 1080.f / 2 - 1080) * -1;
	return Vector2(screenPosX, screenPosY);
}

float Camera::SpriteToObjDir(Vector2 dir)
{
	//角度问题实在是太难了，如果是归一化后的向量，直接tan即可求出弧度(因为长度变成1了)
	//但是这个弧度是和x轴进行比较的，所以还需要减去90度来转到y轴
	//因为弧度实在不容易看出来问题，所以我全部转成了degree，然后输出的时候再转回弧度
	//注意！！数学上，角度增加应该是逆时针旋转，但是我发现引擎是顺时针旋转，所以我的函数也是顺时针
	//所以如果是数学上算出来的角度，使用旋转函数时候必须换成负数！

	float rad = atan2f(dir.y, dir.x);
	float degree = 180.f / acosf(-1) * rad;
	if (degree < 0) {
		degree += 360;
	}
	degree -= 90;
	rad = -(acosf(-1) / 180 * degree);
	return rad;

	//如果有空还是优化一下吧，连续转了两次还是浪费了算力
}
