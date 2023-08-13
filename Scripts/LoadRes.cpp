#include "LoadRes.h"

void LoadRes::LoadResNovice()
{
	_playerSP = Novice::LoadTexture("./Resources/Textures/player.png");
	_bgSP = Novice::LoadTexture("./Resources/Textures/BG.png");

	_map_floor = Novice::LoadTexture("./Resources/Textures/Map_floors.png");
	_map_wall = Novice::LoadTexture("./Resources/Textures/Map_Wall.png");

	_bullet = Novice::LoadTexture("./Resources/Textures/bullet.png");
}
