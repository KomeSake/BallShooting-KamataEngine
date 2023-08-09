#pragma warning(disable: 28251)//无视了主函数WinMain出现的警告
#include <Novice.h>
#include "Scripts/LoadRes.h"
#include "Scripts/Player.h"
#include "Scripts/Camera.h"

const char kWindowTitle[] = "v20230707";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	//我的代码
	LoadRes::LoadResNovice();

	Player* PlayerObj = new Player;
	Camera* CameraObj = new Camera(PlayerObj);


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

		PlayerObj->Move(keys, CameraObj->_bgWidth, CameraObj->_bgHeight);
		CameraObj->Move();



		CameraObj->Show();


		Novice::DrawLine(0, 720 / 2, 1280, 720 / 2, RED);
		Novice::DrawLine(1280 / 2, 0, 1280 / 2, 720, RED);

		Novice::ScreenPrintf(10, 10, "Player(%d,%d)", (int)PlayerObj->_pos.x, (int)PlayerObj->_pos.y);
		Novice::ScreenPrintf(10, 30, "BG(%d,%d)", (int)CameraObj->_bgPos.x, (int)CameraObj->_bgPos.y);
		Novice::ScreenPrintf(10, 50, "Camera(%d,%d)", (int)CameraObj->_cameraPos.x, (int)CameraObj->_cameraPos.y);
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
