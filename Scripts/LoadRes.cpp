#include "LoadRes.h"

void LoadRes::LoadResNovice()
{
	//图片
	_spBullet = { Novice::LoadTexture("./Resources/Textures/bullet.png"),64,64 };
	_spArrow = { Novice::LoadTexture("./Resources/Textures/arrow.png"),384,384 };

	//图集
	int path = 0;
	int listW = 0, listH = 0;
	int w = 0, h = 0, x = 0, y = 0;
	//角色
	for (int i = 0; i < 4; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player.png");
		w = 160, h = 160;
		x = i * w, y = 0;
		listW = 640, listH = 160;
		_spListPlayer[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 4; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player_down.png");
		w = 160, h = 160;
		x = i * w, y = 0;
		listW = 640, listH = 160;
		_spListPlayer_down[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 12; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player_ball.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 1538, listH = 128;
		_spListPlayer_ball[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 7; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player_smoker.png");
		w = 384, h = 384;
		x = i * w, y = 0;
		listW = 2688, listH = 384;
		_spListPlayer_smoker[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 6; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player_ChangeAni.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 768, listH = 128;
		_spListPlayer_ChangeAni[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 6; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Player_ChangeAniTwo.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 768, listH = 128;
		_spListPlayer_ChangeAniTwo[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 5; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Enemy1.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 640, listH = 128;
		_spListEnemy1[i] = { path,x,y,w,h,listW,listH };
	}
	//Map
	for (int i = 0; i < 4; i++) {
		path = Novice::LoadTexture("./Resources/Textures/map2.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 512, listH = 128;
		_spListMap[i] = { path,x,y,w,h,listW,listH };
	}
	//特效
	for (int i = 0; i < 10; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Effect/Explode.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 1280, listH = 128;
		_spListExplode[i] = { path,x,y,w,h,listW,listH };
	}
	for (int i = 0; i < 9; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Effect/EnemyExplode.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 1152, listH = 128;
		_spListEnemyExplode[i] = { path,x,y,w,h,listW,listH };
	}
}
