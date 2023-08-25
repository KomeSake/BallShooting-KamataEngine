#include "Player.h"

Player::Player()
{
	//_pos = { 128 + 128.f / 2,128 + 128.f / 2 };
	_pos = { 2 * 128 - 128.f / 2,15 * 128 - 128.f / 2 };
	_dir = { 0,0 };
	_vel = { 0,0 };
	_acceleration = { 0,0 };
	_speed = 0;
	_bounce = 0;
	_friction = 0;
	_velMax = 0;
	//球形态
	_bounceBall = 0.7f;
	_frictionBall = 0.97f;
	_speedBall = 2.5f;
	_velMaxBall = 20;
	//人形态
	_bounceMan = 0.1f;
	_frictionMan = 0.5f;
	_speedMan = 5;
	_velMaxMan = 15;

	_pattern = 1;
	_hp = 30;
	_bounceValue_enemy = 25;
	_ballDamage = 10;
	_isBallTouch = false;
	_isHarmed = false;
	_isBallEntering = false;
	_isManEntering = false;

	_sprite = LoadRes::_spListPlayer;
	_width = 128;
	_height = 128;
	_scaleX = 1;
	_scaleY = 1;
	_color = WHITE;

	_mousePos.x = 0;
	_mousePos.y = 0;
	_bulletPos = { 0,0 };
	_bulletDir = { 0,0 };
	_bulletTime = 100;

	_steamMax = 500;
	_steamPlusRate = 2.f;
	_steamMinus = 1;
	_steamValue = 100;
	_gunHotMax = 100;
	_gunHotPlus = 10;
	_gunHotMinus = 1;
	_gunHotValue = 0;
	_isGunHot = false;

	_spriteDownDegree = 0;
	_ballStopSprite = 0;
	_isBallStop = false;
	_isDrop = false;
	_dropPos[0] = { 0,0 };
	_dropPos[1] = { 0,0 };
	_dropPos[2] = { 0,0 };
}

void Player::Move(char keys[], vector<vector<char>> mapData, float bgWidth, float bgHeight, float minMapSize)
{
	//移动部分
	if (!_isDrop) {
		if (keys[DIK_W]) {
			_dir.y = 1;
		}
		else if (keys[DIK_S]) {
			_dir.y = -1;
		}
		else {
			_dir.y = 0;
		}
		if (keys[DIK_D]) {
			_dir.x = 1;
		}
		else if (keys[DIK_A]) {
			_dir.x = -1;
		}
		else {
			_dir.x = 0;
		}
		float vectorLength = sqrtf(powf(_dir.x, 2) + powf(_dir.y, 2));
		if (vectorLength != 0) {
			_dir.x = _dir.x / vectorLength;
			_dir.y = _dir.y / vectorLength;
		}

		//格子限制
		//注意！地图格子是二维容器装着的，所以序号是从0开始

		//先计算出当前所在的格子
		int playerCheckRow = (int)((bgHeight - _pos.y) / minMapSize);
		int playerCheckLine = (int)(_pos.x / minMapSize);

		//开始x轴的移动，移动后算出新的上下左右4个角的格子位置
		_acceleration.x = _dir.x * _speed;
		if (_vel.x<_velMax && _vel.x>-_velMax) {
			_vel.x += _acceleration.x;
		}
		_vel.x = _vel.x * _friction;
		_pos.x += _vel.x;
		int checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
		int checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);//下和右格要缩小一像素，这样才准确
		int checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
		int checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
		//通过4个角的格子来判断是否已经不在可通过格子中，如果不在就把player移回去
		if (_vel.x > 0) {
			if (!Map::IsThrough(mapData, checkUp, checkRight)
				|| !Map::IsThrough(mapData, checkDown, checkRight)) {
				_pos.x = playerCheckLine * minMapSize + _width / 2;
				_vel.x = _vel.x * -_bounce;
			}
		}
		else if (_vel.x < 0) {
			if (!Map::IsThrough(mapData, checkUp, checkLeft)
				|| !Map::IsThrough(mapData, checkDown, checkLeft)) {
				_pos.x = playerCheckLine * minMapSize + _width / 2;
				_vel.x = _vel.x * -_bounce;
			}
		}

		//y轴也是相同的，先移动后判断，如果不对劲就把player移回去
		//核心区别就在于，之前都是x轴和y轴都移动了后再进行判断，这样会导致后面的轴被前面影响而导致误判
		_acceleration.y = _dir.y * _speed;
		if (_vel.y<_velMax && _vel.y>-_velMax) {
			_vel.y += _acceleration.y;
		}
		_vel.y = _vel.y * _friction;
		_pos.y += _vel.y;
		checkUp = (int)((bgHeight - _pos.y - _height / 2) / minMapSize);
		checkDown = (int)((bgHeight - _pos.y + _height / 2 - 1) / minMapSize);
		checkLeft = (int)((_pos.x - _width / 2) / minMapSize);
		checkRight = (int)((_pos.x + _width / 2 - 1) / minMapSize);
		if (_vel.y > 0) {
			if (!Map::IsThrough(mapData, checkUp, checkLeft)
				|| !Map::IsThrough(mapData, checkUp, checkRight)) {
				_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_vel.y = _vel.y * -_bounce;
			}
		}
		else if (_vel.y < 0) {
			if (!Map::IsThrough(mapData, checkDown, checkLeft)
				|| !Map::IsThrough(mapData, checkDown, checkRight)) {
				_pos.y = bgHeight - playerCheckRow * minMapSize - _height / 2;
				_vel.y = _vel.y * -_bounce;
			}
		}
		//Novice::ScreenPrintf(10, 100, "u:%d,d:%d,l:%d,r:%d,", checkUp, checkDown, checkLeft, checkRight);

		//最外围的边界限制
		if (_pos.x + _width / 2 > bgWidth) {
			_pos.x = bgWidth - _width / 2;
		}
		else if (_pos.x - _width / 2 < 0) {
			_pos.x = _width / 2;
		}
		if (_pos.y + _height / 2 > bgHeight) {
			_pos.y = bgHeight - _height / 2;
		}
		else if (_pos.y - _height / 2 < 0) {
			_pos.y = _height / 2;
		}
	}
}

void Player::Attack(Vector2 cameraPos)
{
	int x = 0, y = 0;
	Novice::GetMousePosition(&x, &y);
	Vector2 worldPos = ScreenToWorld((float)x, (float)y, cameraPos.x, cameraPos.y);
	_mousePos.x = worldPos.x;
	_mousePos.y = worldPos.y;

	_bulletDir = { _mousePos.x - _pos.x,_mousePos.y - _pos.y };
	float vectorLength = sqrtf(powf(_bulletDir.x, 2) + powf(_bulletDir.y, 2));
	if (vectorLength != 0) {
		_bulletDir.x = _bulletDir.x / vectorLength;
		_bulletDir.y = _bulletDir.y / vectorLength;
	}

	if (_pattern == 1
		&& !_isManEntering
		&& !_isGunHot
		&& !_isDrop) {
		if (Novice::IsPressMouse(0)
			&& MyTimers(_bulletTime, 1)) {
			Bullet* bullet = BulletManager::AcquireBullet(Bullet::normal);
			bullet->Fire(Bullet::Vector2{ _pos.x, _pos.y }, Bullet::Vector2{ _bulletDir.x, _bulletDir.y });

			_gunHotValue += _gunHotPlus;
		}
	}
}

void Player::PatternChange(char keys[], char preKeys[])
{
	//状态变换(按住按键变成球版)(按下鼠标立马变成人形态)
	//现在不是人形态下，子弹是射不出的，如果想用这个版本，就把Attack方法改一下判断条件
	//if (!Novice::IsPressMouse(0)) {
	//	if (keys[DIK_LSHIFT] || keys[DIK_SPACE]) {
	//		_pattern = 0;
	//	}
	//	if (preKeys[DIK_LSHIFT] && !keys[DIK_LSHIFT]) {
	//		_pattern = 1;
	//	}
	//	if (preKeys[DIK_SPACE] && !keys[DIK_SPACE]) {
	//		_pattern = 1;
	//	}
	//}
	//else {
	//	_pattern = 1;
	//}	

	//状态变换(按住按键变成球版)
	if (!preKeys[DIK_LSHIFT] && keys[DIK_LSHIFT]) {
		_isBallEntering = true;
		_frameAniIndex[4] = 0;
		if (_steamValue > 0) {
			_pattern = 0;
			_color = WHITE;
		}
		else {
			_pattern = 2;
			_color = BLUE;
		}
	}
	else if (!preKeys[DIK_SPACE] && keys[DIK_SPACE]) {
		_isBallEntering = true;
		_frameAniIndex[4] = 0;
		if (_steamValue > 0) {
			_pattern = 0;
			_color = WHITE;
		}
		else {
			_pattern = 2;
			_color = BLUE;
		}
	}
	if (preKeys[DIK_LSHIFT] && !keys[DIK_LSHIFT]) {
		_isManEntering = true;
		_frameAniIndex[4] = 0;
		_pattern = 1;
		_color = WHITE;
	}
	else if (preKeys[DIK_SPACE] && !keys[DIK_SPACE]) {
		_isManEntering = true;
		_frameAniIndex[4] = 0;
		_pattern = 1;
		_color = WHITE;
	}
	//状态变换后的属性变化
	switch (_pattern) {
	case 0:
		_speed = _speedBall;
		_bounce = _bounceBall;
		_friction = _frictionBall;
		_velMax = _velMaxBall;
		_width = 98;
		_height = 98;
		break;
	case 1:
		_speed = _speedMan;
		_bounce = _bounceMan;
		_friction = _frictionMan;
		_velMax = _velMaxMan;
		_width = 128;
		_height = 128;
		break;
	case 2:
		_speed = _speedBall / 2;
		_bounce = _bounceBall;
		_friction = _frictionBall;
		_velMax = _velMaxBall / 2;
		_width = 98;
		_height = 98;
		break;
	}
}

void Player::CollideSystem()
{
	switch (_pattern) {
	case 0: {
		//球状态撞敌人
		for (Enemy* element : EnemyManager::_enemyUpdateVector) {
			if (element->_hp > 0) {
				float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
				if (length + 50 < element->_width / 2 + _width / 2) {
					//自身受到的反弹力
					Vector2 hitDir = { _pos.x - element->_pos.x,_pos.y - element->_pos.y };
					hitDir = VectorNormalization(hitDir.x, hitDir.y);
					_vel.x = hitDir.x * _bounceValue_enemy;
					_vel.y = hitDir.y * _bounceValue_enemy;
					//敌人受到的反弹力
					element->_vel.x = -hitDir.x * element->_bounceValue_player;
					element->_vel.y = -hitDir.y * element->_bounceValue_player;

					element->_hp -= _ballDamage;
					element->_isHarmed = true;
					_isBallTouch = true;
				}
			}
		}
		break; }
	case 1:
	case 2: {
		//被敌人碰撞
		for (Enemy* element : EnemyManager::_enemyUpdateVector) {
			float length = sqrtf(powf(element->_pos.x - _pos.x, 2) + powf(element->_pos.y - _pos.y, 2));
			if (length + 50 < element->_width / 2 + _width / 2) {
				Vector2 hitDir = { _pos.x - element->_pos.x,_pos.y - element->_pos.y };
				hitDir = VectorNormalization(hitDir.x, hitDir.y);
				_vel.x = hitDir.x * _bounceValue_enemy;
				_vel.y = hitDir.y * _bounceValue_enemy;

				_isHarmed = true;
				_hp -= element->_damage;
			}
		}
		break; }
	}
}

void Player::DropSystem(vector<vector<char>> mapData, float bgHeight, float minMapSize) {
	//算出中心坐标的格子
	int posCheckLine = (int)((bgHeight - _pos.y) / minMapSize);
	int posCheckRow = (int)(_pos.x / minMapSize);

	//记录下3个生前(跌落前)的坐标(其实记录一个也是可以的，但是保险起见)
	if (mapData[posCheckLine][posCheckRow] != '*') {
		if (MyTimers(1000, 4)) {
			_dropPos[0] = _pos;
		}
		if (MyTimers(2000, 5)) {
			_dropPos[1] = _pos;
		}
		if (MyTimers(3000, 6)) {
			_dropPos[2] = _pos;
		}
	}
	else {
		_isDrop = true;
	}

	float scaleSpeed = 0.03f;//缩小速度
	if (_isDrop) {
		if (_scaleX < 0 || _scaleY < 0) {
			_isDrop = false;
			_isHarmed = true;//借一下受伤效果来用下好了
			_scaleX = 1;
			_scaleY = 1;
			_vel = { 0,0 };
			//跌落到最低，重置玩家位置
			for (int i = 0; i < 3; i++) {
				int line = (int)((bgHeight - _dropPos[i].y) / minMapSize);
				int row = (int)(_dropPos[i].x / minMapSize);
				if (Map::IsThrough(mapData, line, row)&&mapData[line][row]!='*') {
					_pos = _dropPos[i];
					return;
				}
			}
		}
		else {
			//跌落时缩小自身
			_scaleX -= scaleSpeed;
			_scaleY -= scaleSpeed;
			_color = RED;
		}
	}
}

void Player::IsDead()
{
	if (_hp < 0) {
		//这里写上需要的效果
	}
}

void Player::Show()
{
	//其实不用这么麻烦，只需要把对应的上下左右速度算出一个角度值即可
	//然后图片按照这个角度值就可以自然摆出想要的样式了
	switch (_pattern)
	{
	case 0:
	case 2: {
		if (_isBallEntering) {
			float rad = acosf(-1) / 180 * _spriteDownDegree;
			FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ChangeAniTwo, _scaleX, _scaleY, rad, _color, 100, 4);
			if (MyTimers(599, 3)) {
				//形态变换一共6张图600帧，少一帧可以减少卡帧的问题
				_isBallEntering = false;
			}
		}
		else {
			int spriteVel = 5;//图片旋转基准值(速度实际上不会马上变为0,所以需要一个基准值来控制,不然就会觉得图片换的慢)
			if (_vel.x < -spriteVel && _vel.y > spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = -45;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.x > spriteVel && _vel.y > spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = 45;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.x < -spriteVel && _vel.y < -spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = -135;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.x > spriteVel && _vel.y < -spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = 135;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.y > spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = 0;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.y < -spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = 180;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.x < -spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = -90;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else if (_vel.x > spriteVel) {
				_isBallStop = true;
				_spriteDownDegree = 90;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_smoker, _scaleX, _scaleY, rad, WHITE, 70, 3);
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _scaleX, _scaleY, rad, _color, 10, 1);
			}
			else {
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				if (_isBallStop) {
					srand(unsigned int(time(nullptr)));//随机数计算
					_ballStopSprite = rand() % 12;//随机选择停下来球的图片，显得不单调（有12张）
					_isBallStop = false;
				}
				FrameTexture(_pos.x, _pos.y, LoadRes::_spListPlayer_ball, _ballStopSprite, _scaleX, _scaleY, rad, _color);
			}
		}
		break; }

	case 1: {
		if (_isManEntering) {
			float rad = SpriteToObjDir(Vector2{ _bulletDir.x,_bulletDir.y });
			FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_ChangeAni, _scaleX, _scaleY, rad, _color, 100, 4);
			if (MyTimers(599, 3)) {
				//形态变换一共6张图600帧，少一帧可以减少卡帧的问题
				_isManEntering = false;
			}
		}
		else {
			int spriteVel = 1;//图片旋转基准值(速度实际上不会马上变为0,所以需要一个基准值来控制,不然就会觉得图片换的慢)
			if (_vel.x < -spriteVel && _vel.y > spriteVel) {
				_spriteDownDegree = -45;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.x > spriteVel && _vel.y > spriteVel) {
				_spriteDownDegree = 45;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.x < -spriteVel && _vel.y < -spriteVel) {
				_spriteDownDegree = -135;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.x > spriteVel && _vel.y < -spriteVel) {
				_spriteDownDegree = 135;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.y > spriteVel) {
				_spriteDownDegree = 0;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.y < -spriteVel) {
				_spriteDownDegree = 180;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.x < -spriteVel) {
				_spriteDownDegree = -90;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else if (_vel.x > spriteVel) {
				_spriteDownDegree = 90;
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_down, _scaleX, _scaleY, rad, _color, 100, 1);
			}
			else {
				float rad = acosf(-1) / 180 * _spriteDownDegree;
				FrameTexture(_pos.x, _pos.y, LoadRes::_spListPlayer_down, 1, _scaleX, _scaleY, rad, _color);
			}
			//跟随鼠标方向
			float rad = SpriteToObjDir(Vector2{ _bulletDir.x,_bulletDir.y });
			//计算出过热的RGB值
			unsigned int color = (int)(255 - (_gunHotValue * 2.55f));
			if (color < 0) {
				color = 0;
			}
			if (color > 255) {
				color = 255;
			}
			unsigned int colorG = 0, colorB = 0;
			colorG = (colorG >> 16) & 0xFF;
			colorB = (colorB >> 8) & 0xFF;
			colorG += color;
			colorB += color;
			unsigned int gunHotcolor = (255 << 24) | (colorG << 16) | (colorB << 8) | (255);
			if (Novice::IsPressMouse(0) && !_isGunHot) {
				FrameAnimation(_pos.x, _pos.y, LoadRes::_spListPlayer_gun, rad, gunHotcolor, 30, 2);
			}
			else {
				FrameTexture(_pos.x, _pos.y, LoadRes::_spListPlayer_gun, 0, _scaleX, _scaleY, rad, gunHotcolor);
			}
			FrameTexture(_pos.x, _pos.y, LoadRes::_spListPlayer, 0, _scaleX, _scaleY, rad, _color);
			FrameTexture(_pos.x, _pos.y, LoadRes::_spArrow, _scaleX, _scaleY, rad, WHITE);
		}
		break; }
	}
}

void Player::GunHot()
{
	if (_gunHotValue > _gunHotMax) {
		_isGunHot = true;
	}
	if (_isGunHot && _gunHotValue <= 0) {
		_isGunHot = false;
	}
	if (_gunHotValue > 0) {
		_gunHotValue -= _gunHotMinus;
		//转换成蒸汽值
		if (_steamValue < _steamMax) {
			_steamValue += (int)(_gunHotMinus * _steamPlusRate);
		}
	}


	Novice::ScreenPrintf(10, 100, "GunHot:%d/%d", _gunHotValue, _gunHotMax);
}

void Player::Effect()
{
	if (_isHarmed) {
		_color = RED;
		if (MyTimers(100, 2)) {
			if (_steamValue > 0) {
				_color = WHITE;
			}
			else {
				_color = BLUE;
			}
			_isHarmed = false;
		}
	}
}

void Player::SteamPush()
{
	switch (_pattern) {
	case 0:
		if (_steamValue > 0) {
			_steamValue -= _steamMinus;
		}
		else {
			_pattern = 2;
			_color = BLUE;
		}
		break;
	}

	Novice::ScreenPrintf(10, 130, "Steam:%d/%d,type:%d", _steamValue, _steamMax, _pattern);
}
