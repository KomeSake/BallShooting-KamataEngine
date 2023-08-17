#include "Camera.h"

Camera::Camera(const int screenW, const int screenH, int bgW, int bgH, int minMapSize)
{
	_bgWidth = (float)bgW;
	_bgHeight = (float)bgH;
	_screenWidth = (float)screenW;
	_screenHeight = (float)screenH;
	_minMapSize = (float)minMapSize;

	_cameraPos = { _screenWidth / 2,_screenHeight / 2 };
	_bgPos = { _bgWidth / 2,_bgHeight / 2 };
}

void Camera::Move(Vector2 playerPos)
{
	if (playerPos.x >= _screenWidth / 2 && playerPos.x <= _bgWidth - _screenWidth / 2) {
		_cameraPos.x = playerPos.x;
	}
	if (playerPos.y >= _screenHeight / 2 && playerPos.y <= _bgHeight - _screenHeight / 2) {
		_cameraPos.y = playerPos.y;
	}
}

void Camera::Show(Player* PlayerObj)
{
	FrameTexture(PlayerObj->_pos.x, PlayerObj->_pos.y, PlayerObj->_sprite, PlayerObj->_color);
	float rad = SpriteToObjDir(Vector2{ PlayerObj->_bulletDir.x,PlayerObj->_bulletDir.y });
	FrameTexture(PlayerObj->_pos.x, PlayerObj->_pos.y, LoadRes::_spArrow, rad, WHITE);
}

void Camera::MapShow(vector<vector<char>>mapData, float bgW, float bgH, float minSize) {
	bgW;
	Vector2 minMapPos = { 0,0 };
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
				FrameTexture(minMapPos.x, minMapPos.y, LoadRes::_spListMap, 0, WHITE);
				break;
			}
			minMapPos.x += minSize;
		}
		minMapPos.x = minSize / 2;
		minMapPos.y -= minSize;
	}
}

void Camera::BulletShow() {
	for (Bullet* element : BulletManager::_bulletUpdata_player) {
		float rad = SpriteToObjDir(Vector2{ element->_dir.x, element->_dir.y });
		FrameTexture(element->_pos.x, element->_pos.y, element->_sprite, rad, element->_color);
	}
}

void Camera::EnemyShow()
{
	for (Enemy* element : EnemyManager::_enemyUpdateVector) {
		if (element->_isWarning) {
			float rad = SpriteToObjDir(Vector2{ element->_dir.x, element->_dir.y });
			FrameTexture(element->_pos.x, element->_pos.y, element->_sprite, rad, element->_color);
		}
		else {
			srand(unsigned int(time(nullptr)));//随机数计算
			float rad = (float)(rand() % 10);//随机个角度出来，让敌人的未警戒状态看着自然一些
			FrameTexture(element->_pos.x, element->_pos.y, element->_sprite, rad, element->_color);
		}
	}
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
