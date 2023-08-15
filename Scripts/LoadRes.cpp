#include "LoadRes.h"

void LoadRes::LoadResNovice()
{
	//图片
	_spPlayer = { Novice::LoadTexture("./Resources/Textures/player.png"), 128,128 };
	_spBullet = { Novice::LoadTexture("./Resources/Textures/bullet.png"),64,64 };

	//图集
	int path = 0;
	int listW = 0, listH = 0;
	int w = 0, h = 0, x = 0, y = 0;

	for (int i = 0; i < 5; i++) {
		path = Novice::LoadTexture("./Resources/Textures/map2.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 512, listH = 128;
		_spListMap[i] = { path,x,y,w,h,listW,listH };
	}
}
