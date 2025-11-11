#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Grid.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
	:
	grid_(nullptr),
	imgTitle_(-1)
{
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{
	grid_ = new Grid();
	grid_->Init();

	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "ppap.png").c_str());
}

void TitleScene::Update(void)
{
	grid_->Update();

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
	{
		Application::GetInstance().GameEnd();
	}
}

void TitleScene::Draw(void)
{
	//grid_->Draw();

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5f, 0.0, imgTitle_, true);

#pragma region ３Ｄ設定のお試し
	//VECTOR pos;
#pragma region Step1：球体表示(陰影がある)
	//pos = { 0.0f, 0.0f, 0.0f };
	//DrawSphere3D(pos, 60.0f, 10, 0xff0000, 0xff0000, true);
#pragma endregion
#pragma region Step2：ライト無効(陰影がなくなる)
	//SetUseLighting(false);
	//pos = { 0.0f, 0.0f, 0.0f };
	//DrawSphere3D(pos, 60.0f, 10, 0xff0000, 0xff0000, true);
	//SetUseLighting(true);
#pragma endregion
#pragma region Step3：ディレクショナルライトの方向
	//// 角度(回転)制御ではなくて、方向制御なので、
	//// 意図通りには光の方向が動かないもの
	//VECTOR lightDir = GetLightDirection();
	//float pow = 0.05f;
	//if (CheckHitKey(KEY_INPUT_A)) { lightDir.x -= pow; }
	//if (CheckHitKey(KEY_INPUT_D)) { lightDir.x += pow; }
	//if (CheckHitKey(KEY_INPUT_W)) { lightDir.y -= pow; }
	//if (CheckHitKey(KEY_INPUT_S)) { lightDir.y += pow; }
	//SetLightDirection(lightDir);
	//// 真ん中の球体
	//pos = { 0.0f, 0.0f, 0.0f };
	//DrawSphere3D(pos, 60.0f, 10, 0xff0000, 0xff0000, true);
	//// 光の反対方向
	//VECTOR revLightDir = VScale(lightDir, -1.0f);
	//// 光の位置
	//pos = VAdd(pos, VScale(revLightDir, 300.0f));
	//DrawSphere3D(pos, 30.0f, 10, 0xaaaa00, 0xaaaa00, true);
	//DrawLine3D(pos, VAdd(pos, VScale(lightDir, 150.0f)), 0xaaaa00);
	//DrawFormatString(20, 20, 0x000000, 
	// "光の方向：(% .2f, % .2f, % .2f)", lightDir.x, lightDir.y, lightDir.z);
#pragma endregion

#pragma region Step4：Zバッファ無効(前後関係がおかしくなる)
	//SetUseZBuffer3D(false);
	//SetWriteZBuffer3D(false);
	//pos = { 0.0f, 0.0f, 0.0f };
	//DrawSphere3D(pos, 60.0f, 10, 0xff0000, 0xff0000, true);
	//pos = { 0.0f, 0.0f, 100.0f };
	//DrawSphere3D(pos, 100.0f, 10, 0x00ff00, 0x00ff00, true);
	//SetWriteZBuffer3D(true);
	//SetUseZBuffer3D(true);
#pragma endregion

#pragma region Step5：バックカリング
	//// バックカリングを無効にする
	//SetUseBackCulling(false);
	//// カメラ位置
	//pos = { 0.0f, 500.0f, -500.0f };
	//DrawSphere3D(pos, 300.0f, 100, 0xff0000, 0xff0000, true);
	//SetUseBackCulling(true);
	//// ライトの位置変更
	//VECTOR lightDir = GetLightDirection();
	//float pow = 0.05f;
	//if (CheckHitKey(KEY_INPUT_A)) { lightDir.x -= pow; }
	//if (CheckHitKey(KEY_INPUT_D)) { lightDir.x += pow; }
	//if (CheckHitKey(KEY_INPUT_W)) { lightDir.y -= pow; }
	//if (CheckHitKey(KEY_INPUT_S)) { lightDir.y += pow; }
	//SetLightDirection(lightDir);
	//
	//DrawFormatString(20, 20, 0xffffff, 
	// "光の方向：(% .2f, % .2f, % .2f)", lightDir.x, lightDir.y, lightDir.z);
#pragma endregion

#pragma endregion
	DrawString((Application::SCREEN_SIZE_X - GetDrawStringWidth("Space/Aでスタート", strlen("Space?Aでスタート"))) / 2, Application::SCREEN_SIZE_Y - 30, "Space/Aでスタート", GetColor(255, 255, 255));
}

void TitleScene::Release(void)
{
	DeleteGraph(imgTitle_);
}
