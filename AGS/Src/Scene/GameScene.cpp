#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Stage.h"
#include "../Manager/Camera.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{
	stage_ = new Stage();
	stage_->Init();

	// カメラをフリーモードにする
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FREE);

	lightPow_ = 0.001f;
}

void GameScene::Update(void)
{
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	auto cameraAngle = SceneManager::GetInstance().GetCamera().GetAngle();
	Collision();
	CollisionGameOver();

	stage_->Update();

	circlePos_.y += pow;
	circlePos_.x += movePow.x;
	pow -= GRAVITY;

	// バウンド
	if (circlePos_.y - CIRCLE_RADIUS < 0.0f)
	{
		auto& sceneMana = SceneManager::GetInstance();

		circlePos_.y = 0.0f + CIRCLE_RADIUS;

		powdddd /= 2.0f;
		pow = powdddd;
		sceneMana.SetPointLightPos(circlePos_);

		if (!isBound_)
		{
			sceneMana.IsPointLightPow();
			isBound_ = true;
		}
		
	}
	lightPow_ -= 0.001f;
	//SceneManager::GetInstance().SetPointLightPos(lightPow_);

	// 発射キー
	if (CheckHitKey(KEY_INPUT_G) == 1 && !isShooting)
	{
		isBound_ = false;
		isShooting = true;
		startPos_ = cameraPos;
		circlePos_ = cameraPos;
		MATRIX mat = MGetIdent();
		mat = MMult(mat, MGetRotY(cameraAngle.y));
		moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);
		powdddd = 20.0f;
	}

	// 毎フレーム移動
	if (isShooting)
	{
		circlePos_ = VAdd(circlePos_, VScale(moveDir_, powdddd));

		// 発射距離で停止
		VECTOR diff = VSub(circlePos_, startPos_);
		float traveled = VSize(diff);  // 移動距離
		if (traveled >= maxDistance)
		{
			isShooting = false;
		}
	}



}

void GameScene::Draw(void)
{
	stage_->Draw();

	DrawSphere3D(circlePos_, CIRCLE_RADIUS, 10, 0xff0000, 0xff0000, true);
}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;
	stage_ = nullptr;
}

void GameScene::Collision(void)
{
	
}

void GameScene::CollisionGameOver()
{
	
}
