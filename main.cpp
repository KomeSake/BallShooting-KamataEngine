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
#include "Scripts/Scene.h"

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

!!!现在的撞击敌人机制太简单粗暴！要按照球的速度来决定伤害，这样可以让游戏机制跟上一层楼
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
	vector<vector<char>> _mapData;
	int bgWidth = 0;
	int bgHeight = 0;
	int minMapSize = 0;
	int playerBornX = 0;
	int playerBornY = 0;
	Scene* SceneObj = new Scene;
	SceneObj->LocalSave(0);
	Player* PlayerObj = new Player(People::Vector2{ 0 ,0 });
	Camera* CameraObj = new Camera(0, 0, 0, 0, 0);

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

		Novice::DrawBox(0, 0, 1920, 1080, 0, 0x263238ff, kFillModeSolid);//最底下的深渊背景(防止更换场景时候的颜色错误)
		int mouseX = 0, mouseY = 0;
		Novice::GetMousePosition(&mouseX, &mouseY);

		switch (SceneObj->_sceneIndex) {
		case Scene::Loading:
			//初始载入
			BulletManager::_bulletUpdata_player.clear();
			BulletManager::_bulletUpdata_enemy.clear();
			EnemyManager::_enemyUpdateVector.clear();
			switch (SceneObj->_levelNum) {
			case 0:
				_mapData = Map::_mapData_help;
				bgWidth = Map::_mapValue_help[0];
				bgHeight = Map::_mapValue_help[1];
				minMapSize = Map::_mapValue_help[2];
				playerBornX = Map::_mapValue_help[3];
				playerBornY = Map::_mapValue_help[4];
				SceneObj->_sceneIndex = SceneObj->Game;
				break;
			case 1:
				_mapData = Map::_mapData1;
				bgWidth = Map::_mapValue1[0];
				bgHeight = Map::_mapValue1[1];
				minMapSize = Map::_mapValue1[2];
				playerBornX = Map::_mapValue1[3];
				playerBornY = Map::_mapValue1[4];
				SceneObj->_sceneIndex = SceneObj->Game;
				break;
			case 2:
				_mapData = Map::_mapData2;
				bgWidth = Map::_mapValue2[0];
				bgHeight = Map::_mapValue2[1];
				minMapSize = Map::_mapValue2[2];
				playerBornX = Map::_mapValue2[3];
				playerBornY = Map::_mapValue2[4];
				SceneObj->_sceneIndex = SceneObj->Game;
				break;
			case 3:
				_mapData = Map::_mapData3;
				bgWidth = Map::_mapValue3[0];
				bgHeight = Map::_mapValue3[1];
				minMapSize = Map::_mapValue3[2];
				playerBornX = Map::_mapValue3[3];
				playerBornY = Map::_mapValue3[4];
				SceneObj->_sceneIndex = SceneObj->Game;
				break;
			case 4:
				_mapData = Map::_mapData4;
				bgWidth = Map::_mapValue4[0];
				bgHeight = Map::_mapValue4[1];
				minMapSize = Map::_mapValue4[2];
				playerBornX = Map::_mapValue4[3];
				playerBornY = Map::_mapValue4[4];
				SceneObj->_sceneIndex = SceneObj->Game;
				break;
			}
			if (SceneObj->_isStartMapData) {
				_mapData = Map::_mapData_start;
				bgWidth = Map::_mapValue_start[0];
				bgHeight = Map::_mapValue_start[1];
				minMapSize = Map::_mapValue_start[2];
				playerBornX = Map::_mapValue_start[3];
				playerBornY = Map::_mapValue_start[4];
				SceneObj->_sceneIndex = SceneObj->Start;
				SceneObj->_isStartMapData = false;
			}
			PlayerObj = new Player(People::Vector2{ float(playerBornX) ,float(playerBornY) });
			CameraObj = new Camera(screenWidth, screenHeight, bgWidth, bgHeight, minMapSize);
			EnemyManager::EnemyBornToMap(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);
			break;
		case Scene::Start: {
			//开始界面
			EnemyManager::EnemyUpdata(Enemy::Vector2{ float(mouseX), float(mouseY - 1080) * -1 }, _mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			People::CheckCameraValume(People::Vector2{ CameraObj->_cameraPos.x,CameraObj->_cameraPos.y }, screenWidth, screenHeight);
			//鼠标弹开的效果（我好无聊啊，这么紧张的时间还写这些小机制）
			bool isTriggerMouse = false;
			if (Novice::IsTriggerMouse(0)) {
				Camera::Vector2 worldMouse = Camera::ScreenToWorld(float(mouseX), float(mouseY), CameraObj->_cameraPos.x, CameraObj->_cameraPos.y);
				isTriggerMouse = true;
				for (Enemy* element : EnemyManager::_enemyUpdateVector) {
					float length = sqrtf(powf(element->_pos.x - worldMouse.x, 2) + powf(element->_pos.y - worldMouse.y, 2));
					if (length < element->_width / 2) {
						element->_vel.x = -element->_dir.x * element->_bounceValue_player;
						element->_vel.y = -element->_dir.y * element->_bounceValue_player;
					}
				}
			}
			CameraObj->MapShow(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize, false);
			EnemyManager::EnemyShow(false);

			SceneObj->ScreenStart(mouseX, mouseY);
			CameraObj->MouseShow(float(mouseX), float(mouseY), isTriggerMouse);
			break; }
		case Scene::Game: {
			//游玩
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

			Novice::DrawBox(0, 0, 1920, 1080, 0, 0x263238ff, kFillModeSolid);//最底下的深渊背景
			CameraObj->MapShow(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize, PlayerObj->_isExit);
			BulletManager::BulletShow();
			EnemyManager::EnemyShow(true);
			PlayerObj->Effect();
			PlayerObj->Show();

			SceneObj->ScreenGame(PlayerObj);
			PlayerObj->IsExit(_mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			//如果玩家到达终点
			if (PlayerObj->_isExit) {
				SceneObj->_sceneIndex = Scene::LevelClear;
			}
			//如果玩家死亡
			if (PlayerObj->_isDead) {
				SceneObj->_sceneIndex = Scene::GameOver;
			}
			//Boss相关机制
			bool isBossSplit = false;//是否分裂
			int bossSplit = 0;//分裂阶段
			float bornPosX = 0, bornPosY = 0;
			for (Enemy* element : EnemyManager::_enemyUpdateVector) {
				if (element->_type == Enemy::boss) {
					//如果击败Boss
					if (element->_isBossDead) {
						SceneObj->_sceneIndex = Scene::LevelClear;
					}
				}
				if (element->_type == Enemy::boss || element->_type == Enemy::boss2) {
					//Boss分裂
					if (element->_isBossSplit) {
						bornPosX = element->_pos.x + element->_dir.x * 350;
						bornPosY = element->_pos.y + element->_dir.y * 350;
						isBossSplit = true;
						bossSplit = element->_bossSplit;
						element->_isBossSplit = false;
					}
				}
			}
			//因为生成敌人不可以写在敌人循环里面，所以只可以这样了
			if (isBossSplit) {
				Enemy* emeny = EnemyManager::AcquireEnemy(Enemy::boss2);
				emeny->_bossSplit = bossSplit;
				srand(unsigned int(time(nullptr)));
				int pattern = rand() % 1;
				emeny->_bossPattern = pattern;
				emeny->_isBossCrash = true;
				emeny->Fire(Enemy::Vector2{ bornPosX, bornPosY });
			}
			break; }
		case Scene::LevelClear:
			//关卡通过
			EnemyManager::EnemyUpdata(Enemy::Vector2{ float(mouseX), float(mouseY - 1080) * -1 }, _mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			People::CheckCameraValume(People::Vector2{ CameraObj->_cameraPos.x,CameraObj->_cameraPos.y }, screenWidth, screenHeight);
			PlayerObj->IsExit(_mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			PlayerObj->PatternChange(keys, preKeys);

			Novice::DrawBox(0, 0, 1920, 1080, 0, 0x263238ff, kFillModeSolid);//最底下的深渊背景
			CameraObj->MapShow(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize, PlayerObj->_isExit);
			EnemyManager::EnemyShow(false);
			PlayerObj->Show();

			SceneObj->ScreenGame(PlayerObj);
			SceneObj->ScreenLevelClear(mouseX, mouseY);
			break;
		case Scene::GameOver:
			//游戏失败
			EnemyManager::EnemyUpdata(Enemy::Vector2{ float(mouseX), float(mouseY - 1080) * -1 }, _mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			People::CheckCameraValume(People::Vector2{ CameraObj->_cameraPos.x,CameraObj->_cameraPos.y }, screenWidth, screenHeight);
			PlayerObj->IsExit(_mapData, CameraObj->_bgHeight, CameraObj->_minMapSize);
			PlayerObj->PatternChange(keys, preKeys);

			Novice::DrawBox(0, 0, 1920, 1080, 0, 0x263238ff, kFillModeSolid);//最底下的深渊背景
			CameraObj->MapShow(_mapData, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize, PlayerObj->_isExit);
			EnemyManager::EnemyShow(false);
			PlayerObj->Show();

			SceneObj->ScreenGame(PlayerObj);
			SceneObj->ScreenDead(mouseX, mouseY);
			break;
		}
		//隐藏鼠标，换成用自己的(鼠标贴图不知道为什么有点偏移，我加了补偿距离了)
		Novice::SetMouseCursorVisibility(0);
		Novice::DrawSprite(mouseX - 64 + 10, mouseY - 64 + 15, LoadRes::_spArrow_mouse.path, 0.8f, 0.8f, 0, WHITE);

		//退出的时候保存一下解说关卡信息
		if (SceneObj->_isExitButton) {
			SceneObj->LocalSave(1);
			break;
		}

		//调试信息
		//int path = Novice::LoadTexture("./Resources/Textures/temp.png");
		//Novice::DrawSprite(0, 0, path, 1, 1, 0, WHITE);
		//Novice::DrawLine(0, (int)(CameraObj->_screenHeight / 2), (int)CameraObj->_screenWidth, (int)(CameraObj->_screenHeight / 2), RED);
		//Novice::DrawLine((int)(CameraObj->_screenWidth / 2), 0, (int)(CameraObj->_screenWidth / 2), (int)CameraObj->_screenHeight, RED);

		//Novice::ScreenPrintf(10, 10, "Player(%d,%d)", (int)PlayerObj->_pos.x, (int)PlayerObj->_pos.y);
		//Novice::ScreenPrintf(10, 30, "Camera(%d,%d)", (int)CameraObj->_cameraPos.x, (int)CameraObj->_cameraPos.y);
		//Novice::ScreenPrintf(10, 50, "PlayerRL(%d,%d)", (int)((bgHeight - PlayerObj->_pos.y) / minMapSize), (int)(PlayerObj->_pos.x / minMapSize));
		//Novice::ScreenPrintf(10, 70, "level:%d,lock:%d", SceneObj->_levelNum, SceneObj->_levelLockNum);


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			SceneObj->LocalSave(1);
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
