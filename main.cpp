#pragma warning(disable: 28251)//无视了主函数WinMain出现的警告
#include <Novice.h>
#include "Scripts/LoadRes.h"
#include "Scripts/Player.h"
#include "Scripts/Camera.h"
#include "Scripts/Map.h"

const char kWindowTitle[] = "v20230707";
const int screenWidth = 1920;
const int screenHeight = 1080;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, screenWidth, screenHeight);
	Novice::SetWindowMode(kWindowed);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//我的代码
	int bgWidth = 5760;
	int bgHeight = 3200;
	int minMapSize = 128;

	LoadRes::LoadResNovice();
	Map::LoadNovice();

	Player* PlayerObj = new Player;
	Camera* CameraObj = new Camera(PlayerObj, screenWidth, screenHeight, bgWidth, bgHeight, minMapSize);


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

		PlayerObj->Move(keys, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);
		CameraObj->Move();


		CameraObj->MapShow(Map::_mapData1, CameraObj->_bgWidth, CameraObj->_bgHeight, CameraObj->_minMapSize);
		CameraObj->Show();


		Novice::DrawLine(0, (int)(CameraObj->_screenHeight / 2), (int)CameraObj->_screenWidth, (int)(CameraObj->_screenHeight / 2), RED);
		Novice::DrawLine((int)(CameraObj->_screenWidth / 2), 0, (int)(CameraObj->_screenWidth / 2), (int)CameraObj->_screenHeight, RED);

		Novice::ScreenPrintf(10, 10, "Player(%d,%d)", (int)PlayerObj->_pos.x, (int)PlayerObj->_pos.y);
		Novice::ScreenPrintf(10, 30, "BG(%d,%d)", (int)CameraObj->_bgPos.x, (int)CameraObj->_bgPos.y);
		Novice::ScreenPrintf(10, 50, "Camera(%d,%d)", (int)CameraObj->_cameraPos.x, (int)CameraObj->_cameraPos.y);
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
