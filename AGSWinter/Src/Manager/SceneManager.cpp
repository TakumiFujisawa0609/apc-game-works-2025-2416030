//-------------------------------------------------------------------
// ゲーム制作：AGS
// 氏名　　　：2416030　野本　アレン
//-------------------------------------------------------------------
// SceneManager class
#include <DxLib.h>
#include "SceneManager.h"
#include "../Scene/GameScene.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameOverScene.h"
#include "../Scene/GameClearScene.h"
#include "../StDefineData.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->SystemInit();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

SceneManager::SceneManager()
{
	scene_ID = E_SCENE_NON;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;
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
bool SceneManager::SystemInit(void)
{
	SetTransColor(0xff, 0x00, 0xff);		//透過色の設定

	// 背景色設定
	SetBackgroundColor(0, 0, 0);

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();
	DeltaTimeInit();	// デルタタイムの初期化

	ChangeScene(E_SCENE_TITLE);

	return true;
}

/*
* ----------------------------------------------
* ゲーム起動・再開時に必ず呼び出す処理
*	Input:
*		なし
*	Output:
*		なし
* ----------------------------------------------
*/
void SceneManager::GameInit(void)
{
	
}

/*
* ----------------------------------------------
* 更新処理
*	Input:
*		なし
*	Output:
*		なし
* ----------------------------------------------
*/
void SceneManager::Update(void)
{
	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	DeltaTimeUpdate();	// デルタタイムの更新

	totalTime_ += deltaTime_;

	E_SCENE_ID nextSceneID = scene_ID;

	scene_->Update();
	nextSceneID = scene_->GetNextSceneID();

	if (scene_ID != nextSceneID) {		// シーン遷移
		ChangeScene(nextSceneID);
	}
}

/*
* ----------------------------------------------
* 描画処理
*	Input:
*		なし
*	Output:
*		なし
* ----------------------------------------------
*/
void SceneManager::Draw(void)
{
	SetDrawScreen(DX_SCREEN_BACK);			// 描画する画面を裏の画面に設定する
	ClearDrawScreen();						// 描画する画面の内容を消去する

	scene_->Draw();
}

/*
* ----------------------------------------------
* 解放処理  （最初の一回のみ実行）
*	Input:
*		なし
*	Output:
*		bool true = 正常終了 / false = 異常終了
* ----------------------------------------------
*/
bool SceneManager::Release(void)
{
	delete scene_;
	scene_ = nullptr;

	return true;
}

bool SceneManager::ChangeScene(E_SCENE_ID id)
{
	// シーンを変更する
	scene_ID = id;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (scene_ID)
	{
	case E_SCENE_ID::E_SCENE_TITLE:
		scene_ = new TitleScene();
		break;
	case E_SCENE_ID::E_SCENE_GAME:
		scene_ = new GameScene();
		break;
	case E_SCENE_ID::E_SCENE_GAMEOVER:
		scene_ = new GameOverScene();
		break;
	case E_SCENE_ID::E_SCENE_GAMECLEAR:
		scene_ = new GameClearScene();
		
		break;
	}

	scene_->Init();

	// デルタタイムの初期化
	ResetDeltaTime();

	return true;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	//return deltaTime_;
	return DeltaTime;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DeltaTimeInit(void)
{
	// 現在のシステム時間を取得
	NowTime = GetNowHiPerformanceCount();

	// システム時間を取得しておく
	Time = GetNowHiPerformanceCount();

	// 最初の経過時間は仮に 0.000001f 秒にしておく
	DeltaTime = 0.000001f;

	// FPS計測関係の初期化
	FPSCheckTime = GetNowHiPerformanceCount();
	FPS = 0;
	FPSCounter = 0;
}

void SceneManager::DeltaTimeUpdate(void)
{
	// 現在のシステム時間を取得
	NowTime = GetNowHiPerformanceCount();

	// 前回取得した時間からの経過時間を秒に変換してセット
	// ( GetNowHiPerformanceCount で取得できる値はマイクロ秒単位なので 1000000 で割ることで秒単位になる )
	DeltaTime = (NowTime - Time) / 1000000.0f;

	// 今回取得した時間を保存
	Time = NowTime;

	// FPS関係の処理( 1秒経過する間に実行されたメインループの回数を FPS とする )
	FPSCounter++;
	if (NowTime - FPSCheckTime > 1000000)
	{
		FPS = FPSCounter;
		FPSCounter = 0;
		FPSCheckTime = NowTime;
	}
}