#pragma warning(disable: 28251)//无视了主函数WinMain出现的警告
#include <Novice.h>
#include "Scripts/LoadRes.h"
#include "Scripts/People.h"
#include "Scripts/Player.h"
#include "Scripts/Enemy.h"
#include "Scripts/ItemBase.h"
#include "Scripts/Bullet.h"
#include "Scripts/Camera.h"
#include "Scripts/Map.h"
#include "Scripts/GameUI.h"

/*
还需要制作的功能
1、状态转变动画，进入球和退出球。为了让玩家的转换不要这么瞬时，给一点小小的时间惩罚
2、武器过热 -> 充能系统。初步想法是玩家需要攻击才会给自己充能，这样才可以用球去撞别人
3、蒸汽冲刺。可以蓄力消耗更多的蒸汽来一波大的、
3、敌人陨落，敌人被撞击动画。敌人如果被装出平台，应该直接死亡，同时要有相应的动画展示。还有敌人被撞应该有一个放大缩小的效果比较适合
4、敌人种类增加。敌人肯定不能只有这些，不然太单调了
4、敌人掉落，敌人会掉落燃料和子弹等等
5、Boss。如果想弄成关卡制，那么Boss肯定也是需要的了
6、地图设计，美术设计。现在是制作好了地图读取系统，但是地图还没设计好，还有美术也需要添加
7、杂项：UI、关卡轮回（怎么感觉还挺多东西的）
8、地图机制，像是把人推开等等的
*/


//KamataEngine_v20230707
const char kWindowTitle[] = "BallShooting（射撃之丸）";
const int screenWidth = 1920;
const int screenHeight = 1080;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, screenWidth, screenHeight);
	Novice::SetWindowMode(kFullscreen);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//我的代码
	LoadRes::LoadResNovice();
	Map::LoadNovice();
	vector<vector<char>> _mapData = Map::_mapData1;
	int bgWidth = Map::_mapValue1[0];
	int bgHeight = Map::_mapValue1[1];
	int minMapSize = Map::_mapValue1[2];

	Player* PlayerObj = new Player;
	Camera* CameraObj = new Camera(screenWidth, screenHeight, bgWidth, bgHeight, minMapSize);
	EnemyManager::EnemyBornToMap(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);

	PlayerUI_HP* UI_HPObj = new PlayerUI_HP(PlayerObj);
	PlayerUI_Gun* UI_GunObj = new PlayerUI_Gun;
	PlayerUI_Steam* UI_SteamObj = new PlayerUI_Steam;

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		BulletManager::BulletUpdata(Bullet::Vector2{ CameraObj->_cameraPos.x, CameraObj->_cameraPos.y }, _mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
		EnemyManager::EnemyUpdata(Enemy::Vector2{ PlayerObj->_pos.x, PlayerObj->_pos.y }, _mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);

		PlayerObj->PatternChange(keys, preKeys);
		PlayerObj->CollideSystem();
		if (PlayerObj->_isBallTouch) {
			//这个相机的特效方法有点问题，有时候并没有晃动
			if (CameraObj->CameraEffect(0)) {
				PlayerObj->_isBallTouch = false;
			}
		}
		PlayerObj->Move(keys, _mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);
		PlayerObj->DropSystem(_mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
		CameraObj->Move(Camera::Vector2{ PlayerObj->_pos.x, PlayerObj->_pos.y });
		People::CheckCameraValume(People::Vector2{ CameraObj->_cameraPos.x,CameraObj->_cameraPos.y }, screenWidth, screenHeight);
		ItemBase::CheckCameraValume(ItemBase::Vector2{ CameraObj->_cameraPos.x,CameraObj->_cameraPos.y }, screenWidth, screenHeight);
		PlayerObj->Attack(People::Vector2{ CameraObj->_cameraPos.x, CameraObj->_cameraPos.y });
		PlayerObj->GunHot();
		PlayerObj->SteamPush();
		PlayerObj->IsDead();


		CameraObj->MapShow(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);
		EnemyManager::EnemyShow();
		BulletManager::BulletShow();
		PlayerObj->Effect();
		PlayerObj->Show();

		UI_HPObj->UIOpen(PlayerObj);
		UI_GunObj->UIOpen(PlayerObj);
		UI_SteamObj->UIOpen(PlayerObj);

		//调试信息
		//Novice::DrawLine(0, (int)(CameraObj->_screenHeight / 2), (int)CameraObj->_screenWidth, (int)(CameraObj->_screenHeight / 2), RED);
		//Novice::DrawLine((int)(CameraObj->_screenWidth / 2), 0, (int)(CameraObj->_screenWidth / 2), (int)CameraObj->_screenHeight, RED);

		Novice::ScreenPrintf(10, 10, "Player(%d,%d)", (int)PlayerObj->_pos.x, (int)PlayerObj->_pos.y);
		Novice::ScreenPrintf(10, 30, "Camera(%d,%d)", (int)CameraObj->_cameraPos.x, (int)CameraObj->_cameraPos.y);
		Novice::ScreenPrintf(10, 70, "PlayerRL(%d,%d)", (int)((bgHeight - PlayerObj->_pos.y) / minMapSize), (int)(PlayerObj->_pos.x / minMapSize));


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
