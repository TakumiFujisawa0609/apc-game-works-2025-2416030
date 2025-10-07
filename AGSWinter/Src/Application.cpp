//-------------------------------------------------------------------
// ゲーム制作：AGS
// 氏名　　　：2416030　野本　アレン
//-------------------------------------------------------------------
// Application class
#include <DxLib.h>
#include <time.h>
#include "Application.h"
#include "Manager/SceneManager.h"
#include "Common/Mouse.h"
#include "Common/Ranking.h"

const std::string Application::PATH_DATA = "Data/";
const std::string Application::PATH_IMAGE = PATH_DATA + "Image/";
const std::string Application::PATH_MODEL = PATH_DATA + "Model/";

Application::Application(void)
{
	
}

Application::~Application(void)
{

}

/*
* ----------------------------------------------
* 初期化処理（最初の一回のみ実行）
*	Input:
*		なし
*	Output:
*		bool true = 正常終了 / false = 異常終了
* ----------------------------------------------
*/
bool Application::SystemInit(void)
{
	//---------------------------
	// システム処理
	
	SetWindowText("AGS");
	
#if 1
	SetGraphMode(SCREEN_SIZE_WID2, SCREEN_SIZE_HIG2, 32);
	ChangeWindowMode(true);
#else
	// フルスクリーン
	SetGraphMode(SCREEN_SIZE_WID, SCREEN_SIZE_HIG, 32);
#if 1
	ChangeWindowMode(false);
#else
	ChangeWindowMode(true); // ウィンドウモード
#endif
#endif

	// ScreenFlip を実行しても垂直同期信号を待たない
	//SetWaitVSyncFlag(FALSE);

	// DxLibの初期化
	SetUseDirect3DVersion(DX_DIRECT3D_11);

	// ＤＸライブラリの初期化
	if (DxLib_Init() == -1) return false;

	// 乱数の初期化
	srand((unsigned int)time(NULL));

	// インスタンスの作成
	Mouse::CreateInstance();
	Ranking::CreateInstance();

	// シーン管理初期化
	SceneManager::CreateInstance();

	SetUseZBuffer3D(TRUE);             // Zバッファを使う
	SetWriteZBuffer3D(TRUE);           // Zバッファへの書き込みもON
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255); // アルファブレンドモード

	return true;
}

/*
* ----------------------------------------------
* ゲーム起動
*	Input:
*		なし
*	Output:
*		なし
* ----------------------------------------------
*/
void Application::Run(void)
{
	SceneManager& sceneManager = SceneManager::GetInstance();

	//---------------------------
	// ゲームループ
	//---------------------------
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0) {
		sceneManager.Update();

		sceneManager.Draw();
		Mouse::GetInstance().Draw();

		ScreenFlip();							// 裏の画面を表の画面に瞬間コピー
	}
}

bool Application::Release(void)
{
	// シーン管理解放
	SceneManager::GetInstance().Release();

	Mouse::GetInstance().Release();

	if (DxLib_End() == -1)return false;

	return true;
}