#include <EffekseerForDXLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/GameScene.h"
#include "../Scene/GameOver.h"
#include "../Scene/GameClear.h"
#include "SceneManager.h"
#include "Camera.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::DeleteInstance(void)
{
	delete instance_;
	instance_ = nullptr;
}

void SceneManager::Init(void)
{

	sceneId_ = SCENE_ID::TITLE;
	waitSceneId_ = SCENE_ID::NONE;

	// フェード機能の初期化
	fader_ = new Fader();
	fader_->Init();

	camera_ = new Camera();
	camera_->Init();


	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	// 3D要の設定
	Init3D();

	// 初期シーンの設定
	DoChangeScene(SCENE_ID::TITLE);

	lightPow_ = 0.001f;
}

void SceneManager::Init3D(void)
{
	// 背景色設定
	SetBackgroundColor(0, 139, 139);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(false);

	// ライトの設定
	SetUseLighting(TRUE); // ← これを有効に！
	SetLightEnable(TRUE); // ← これもセットで！

	// ディレクショナルライト方向の設定（正規化されてなくてもいい）
	// 正面から斜め下に向かったライト
	//ChangeLightTypeDir({ 0.00f, -1.00f, 1.00f });

#if 1
#pragma region Step1 ポイントライト
	int lightID = -1;

	pointLightPos_ = { 0.0f, 40.0f, 180.0f };
	/*ChangeLightTypePoint(
		pointLightPos_, 400.0f, 0.000f, 0.001f, 0.000f);*/

	lightID = ChangeLightTypePoint(pointLightPos_, 400.0f, 0.000f, 0.001f, 0.000f);
	SetLightEnable(true);

#pragma endregion
#else
#pragma region Step2 スポットライト
	spotLightPos_ = { 500.0f, 300.0f, 45.0f };
	ChangeLightTypeSpot(
		spotLightPos_,
		{ 0.0f, 0.0f, 1.0f },
		360.0f * DX_PI_F / 180.0f,
		0.0f * DX_PI_F / 180.0f,
		200.0f,
		0.000f, 0.001f, 0.000f);
#pragma endregion
#endif

		// フォグ設定
	SetFogEnable(true);

	// フォグの色
	SetFogColor(100, 100, 100);

	// フォグを発生させる奥行きの最小、最大距離
	SetFogStartEnd(0 , 8000);

	DeltaTimeInit();
}

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	DeltaTimeUpdate();

	// フェード機能の更新
	fader_->Update();
	if (isSceneChanging_)
	{
		// フェード状態の切替処理
		Fade();
	}
	else
	{
		// 各シーンの更新処理
		scene_->Update();
	}
	UpdateLight();
}

void SceneManager::UpdateLight(void)
{
	if (lightPow_ < 0.01f)
	{
		// 徐々に暗く
		lightPow_ += 0.00004f * GetDeltaTime();
	}
	else
	{
		// 真っ暗
		lightPow_ = 0.08f;
		camera_->SetFarClip(100.0f);
	}
	SetLightRangeAtten(400.0f, 0.000001f, lightPow_, 0.0000001f);

	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		if (lights_[i].isActive == false) continue;

		SetLightEnableHandle(lights_[i].handle, TRUE);
	}
}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();
	if(sceneId_ == SCENE_ID::GAME)
	{
		//if(scene_->CollisionCamera())
		{
			// カメラ設定
			camera_->SetBeforeDraw();
		}
	}

	// Effekseerにより再生中のエフェクトを更新する
	UpdateEffekseer3D();

	// 各シーンの描画処理
	scene_->Draw();

	// 暗転・明転
	fader_->Draw();

#pragma region Step1 ポイントライト
	/*if (CheckHitKey(KEY_INPUT_T)) { pointLightPos_.z = 3.0f; }
	if (CheckHitKey(KEY_INPUT_G)) { pointLightPos_.z -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_R)) { pointLightPos_.y += 3.0f; }
	if (CheckHitKey(KEY_INPUT_Y)) { pointLightPos_.y -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_H)) { pointLightPos_.x += 3.0f; }
	if (CheckHitKey(KEY_INPUT_F)) { pointLightPos_.x -= 3.0f; }*/
	
	SetLightPosition(pointLightPos_);
	
	SetLightRangeAtten(400.0f, 0.000001f, lightPow_, 0.0000001f);
	// 標準ライトのディフューズカラーを青色にする
	//SetLightDifColor(GetColorF(255.0f, 255.0f, 255.0f, 0.0f));

#ifdef _DEBUG
	DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS : %.1f", 1.0f / deltaTime_);
#endif // DEBUG

#pragma endregion
#pragma region Step2 スポットライト
	/*if (CheckHitKey(KEY_INPUT_T)) { spotLightPos_.z += 3.0f; }
	if (CheckHitKey(KEY_INPUT_G)) { spotLightPos_.z -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_R)) { spotLightPos_.y += 3.0f; }
	if (CheckHitKey(KEY_INPUT_Y)) { spotLightPos_.y -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_H)) { spotLightPos_.x += 3.0f; }
	if (CheckHitKey(KEY_INPUT_F)) { spotLightPos_.x -= 3.0f; }
	SetLightPosition(spotLightPos_);
	DrawSphere3D(spotLightPos_, 20.0f, 10, 0xff0000, 0xff0000, true);*/
#pragma endregion

	SetUseBackCulling(FALSE);
	//DrawSphere3D(pointLightPos_, 80.0f, 10, GetColor(255, 255, 0), GetColor(255, 255, 0), TRUE);
	SetUseBackCulling(TRUE);
}

void SceneManager::Destroy(void)
{

	// シーンの解放
	scene_->Release();
	delete scene_;

	// フェード機能の解放
	delete fader_;

	// インスタンスのメモリ解放
	delete instance_;

	// ライトハンドルの削除
	DeleteLightHandle(pointLight1_);
	DeleteLightHandle(pointLight2_);
	DeleteLight();

}

void SceneManager::ChangeScene(SCENE_ID nextId)
{

	// フェード処理が終わってからシーンを変える場合もあるため、
	// 遷移先シーンをメンバ変数に保持
	waitSceneId_ = nextId;

	// フェードアウト(暗転)を開始する
	fader_->SetFade(Fader::STATE::FADE_OUT);
	isSceneChanging_ = true;

}

SceneManager::SCENE_ID SceneManager::GetSceneID(void)
{
	return sceneId_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	//return deltaTime_;

	// 60FPS固定
	return DeltaTime * 60.0f;
}

void SceneManager::CreateLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].handle = CreatePointLightHandle({ 0,0,0 }, 0.0f, 0.0f, 0.0f, 0.0f);
		lights_[i].isActive = true;
		SetLightEnableHandle(lights_[i].handle, FALSE);
	}
}

void SceneManager::DeleteLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].handle = -1;
		lights_[i].isActive = false;
	}
}

void SceneManager::CreateLight()
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		if (lights_[i].isActive == true) continue;

		SetLightEnableHandle(lights_[i].handle, TRUE);
	}
}

SceneManager::SceneManager(void)
{

	sceneId_ = SCENE_ID::NONE;
	waitSceneId_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

	CreateLight();
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(SCENE_ID sceneId)
{

	// シーンを変更する
	sceneId_ = sceneId;

	// 現在のシーンを解放
	if (scene_ != nullptr)
	{
		scene_->Release();
		delete scene_;
	}

	switch (sceneId_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;
	case SCENE_ID::GAMEOVER:
		scene_ = new GameOver();
		break;
	case SCENE_ID::CLEAR:
		scene_ = new GameClear();
		break;
	}

	// 各シーンの初期化
	scene_->Init();
	camera_->Init();

	ResetDeltaTime();

	waitSceneId_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		// 明転中
		if (fader_->IsEnd())
		{
			// 明転が終了したら、フェード処理終了
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		// 暗転中
		if (fader_->IsEnd())
		{
			// 完全に暗転してからシーン遷移
			DoChangeScene(waitSceneId_);
			// 暗転から明転へ
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

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