#include "LoadRes.h"

void LoadRes::LoadResNovice()
{
	//图片
	_spBullet = { Novice::LoadTexture("./Resources/Textures/bullet.png"),64,64 };
	_spArrow = { Novice::LoadTexture("./Resources/Textures/arrow.png"),384,384 };
	_spArrow_mouse = { Novice::LoadTexture("./Resources/Textures/arrow_mouse.png"),128,128 };

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
		path = Novice::LoadTexture("./Resources/Textures/Player_gun.png");
		w = 160, h = 160;
		x = i * w, y = 0;
		listW = 640, listH = 160;
		_spListPlayer_gun[i] = { path,x,y,w,h,listW,listH };
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
	for (int i = 0; i < 6; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Enemy2.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 640, listH = 128;
		_spListEnemy2[i] = { path,x,y,w,h,listW,listH };
	}
	//Map
	for (int i = 0; i < 4; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Map/List.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 512, listH = 128;
		_spListMap[i] = { path,x,y,w,h,listW,listH };
	}
	path = Novice::LoadTexture("./Resources/Textures/Map/HelpText.png");
	listW = 1717, listH = 440;
	w = 436, h = 180, x = 7, y = 10;
	_spListHelpText[0] = { path,x,y,w,h,listW,listH };
	w = 513, h = 200, x = 482, y = 0;
	_spListHelpText[1] = { path,x,y,w,h,listW,listH };
	w = 423, h = 132, x = 1064, y = 12;
	_spListHelpText[2] = { path,x,y,w,h,listW,listH };
	w = 503, h = 194, x = 0, y = 246;
	_spListHelpText[3] = { path,x,y,w,h,listW,listH };
	w = 345, h = 170, x = 553, y = 232;
	_spListHelpText[4] = { path,x,y,w,h,listW,listH };
	w = 345, h = 116, x = 950, y = 244;
	_spListHelpText[5] = { path,x,y,w,h,listW,listH };
	w = 383, h = 162, x = 1334, y = 230;
	_spListHelpText[6] = { path,x,y,w,h,listW,listH };
	for (int i = 0; i < 13; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Map/Exit.png");
		w = 384, h = 384;
		x = i * w, y = 0;
		listW = 4992, listH = 384;
		_spListExit[i] = { path,x,y,w,h,listW,listH };
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
	for (int i = 0; i < 4; i++) {
		path = Novice::LoadTexture("./Resources/Textures/Effect/BulletExplode.png");
		w = 128, h = 128;
		x = i * w, y = 0;
		listW = 512, listH = 128;
		_spListBulletExplode[i] = { path,x,y,w,h,listW,listH };
	}

	//UI
	_spStart = { Novice::LoadTexture("./Resources/Textures/UI/start.png"),437,698 };

	path = Novice::LoadTexture("./Resources/Textures/UI/HP/List.png");
	listW = 700, listH = 404;
	w = 240, h = 404, x = 0, y = 0;
	_spUI_playerHp[0] = { path,x,y,w,h,listW,listH };
	w = 460, h = 72, x = 240, y = 0;
	_spUI_playerHp[1] = { path,x,y,w,h,listW,listH };
	//2:a75f53
	w = 404, h = 44, x = 240, y = 72;
	_spUI_playerHp[3] = { path,x,y,w,h,listW,listH };

	path = Novice::LoadTexture("./Resources/Textures/UI/Gun/List.png");
	listW = 1152, listH = 264;
	w = 648, h = 264, x = 0, y = 0;
	_spUI_playerGun[0] = { path,x,y,w,h,listW,listH };
	w = 252, h = 92, x = 648, y = 0;
	_spUI_playerGun[1] = { path,x,y,w,h,listW,listH };
	w = 252, h = 92, x = 900, y = 0;
	_spUI_playerGun[2] = { path,x,y,w,h,listW,listH };
	_spUI_playerGun02 = { Novice::LoadTexture("./Resources/Textures/UI/Gun/List02.png") ,252,92 };

	path = Novice::LoadTexture("./Resources/Textures/UI/Steam/List.png");
	listW = 1664, listH = 124;
	w = 876, h = 124, x = 0, y = 0;
	_spUI_playerSteam[0] = { path,x,y,w,h,listW,listH };
	w = 788, h = 32, x = 876, y = 48;
	_spUI_playerSteam[1] = { path,x,y,w,h,listW,listH };
	w = 592, h = 48, x = 876, y = 0;
	_spUI_playerSteam[2] = { path,x,y,w,h,listW,listH };
	w = 60, h = 36, x = 1468, y = 0;
	_spUI_playerSteam[3] = { path,x,y,w,h,listW,listH };
	_spUI_playerSteam01 = { Novice::LoadTexture("./Resources/Textures/UI/Steam/List01.png") ,788,32 };

	path = Novice::LoadTexture("./Resources/Textures/UI/Start/List.png");
	listW = 1788, listH = 284;
	w = 944, h = 264, x = 0, y = 0;
	_spUI_screenStart[0] = { path,x,y,w,h,listW,listH };
	w = 716, h = 172, x = 944, y = 0;
	_spUI_screenStart[1] = { path,x,y,w,h,listW,listH };
	w = 308, h = 112, x = 944, y = 172;
	_spUI_screenStart[2] = { path,x,y,w,h,listW,listH };
	w = 308, h = 112, x = 1252, y = 172;
	_spUI_screenStart[3] = { path,x,y,w,h,listW,listH };
	w = 128, h = 64, x = 1660, y = 0;
	_spUI_screenStart[4] = { path,x,y,w,h,listW,listH };
	w = 128, h = 64, x = 1660, y = 64;
	_spUI_screenStart[5] = { path,x,y,w,h,listW,listH };
}
