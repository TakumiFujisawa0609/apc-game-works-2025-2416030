#include "../Utility/AsoUtility.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Stage.h"
#include "../Object/Enemy.h"
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

	enemy_ = new Enemy();
	enemy_->Init();

	// カメラをフリーモードにする
	SceneManager::GetInstance().GetCamera().ChangeMode(Camera::MODE::FREE);

	lightPow_ = 0.001f;

	seId_ = LoadSoundMem((Application::PATH_SE + "Aura.mp3").c_str());
}

void GameScene::Update(void)
{
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
	auto cameraAngle = SceneManager::GetInstance().GetCamera().GetAngle();
	bool isCollision = Collision();
	bool isCameraCollision = CollisionCamera();

	stage_->Update();
	enemy_->Update();

	if (isCollision)
	{
		auto& sceneMana = SceneManager::GetInstance();

		//int a = 0;
		if (!isCollision_)
		{
			sceneMana.SetPointLightPos(circlePos_);
			sceneMana.IsPointLightPow();
			isCollision_ = true;
			SceneManager::GetInstance().GetCamera().SetFarClip(1800.0f);
			PlaySoundMem(seId_, DX_PLAYTYPE_BACK);
		}
	}
	else
	{
		//// 毎フレーム移動
		//if (isShooting)
		//{
		//	

		//	//// 発射距離チェック
		//	//VECTOR diff = VSub(circlePos_, startPos_);
		//	//float traveled = VSize(diff);
		//	//if (traveled >= maxDistance)
		//	//{
		//	//	isShooting = false;
		//	//	powdddd = 0.0f;  // 念のため速度リセット
		//	//}
		//}
		// 移動
		circlePos_ = VAdd(circlePos_, VScale(moveDir_, powdddd));

		circlePos_.y += pow;
		circlePos_.x += movePow.x;
		pow -= GRAVITY;
	}

	auto& ins = InputManager::GetInstance();
	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	

	// 発射キー
	if (CheckHitKey(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		isCollision_ = false;
		isShooting = true;
		//startPos_ = cameraPos;

		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotY(cameraAngle.y));
		//moveDir_ = VTransform({ 0.0f,0.0f,5.0f }, mat);


		mat = MGetIdent();
		mat = MMult(mat, MGetRotX(cameraAngle.x));
		mat = MMult(mat, MGetRotY(cameraAngle.y));
		//moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);
		moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);

		circlePos_ = VAdd(cameraPos, moveDir_);

		pow = 20.0f;
		powdddd = 20.0f;  // 移動速度
		
	}
	
	//// バウンド
	//if (circlePos_.y - CIRCLE_RADIUS < 0.0f)
	//{
	//	auto& sceneMana = SceneManager::GetInstance();

	//	circlePos_.y = 0.0f + CIRCLE_RADIUS;

	//	powdddd /= 2.0f;
	//	pow = powdddd;
	//	sceneMana.SetPointLightPos(circlePos_);

	//	if (!isBound_)
	//	{
	//		sceneMana.IsPointLightPow();
	//		isBound_ = true;
	//	}
	//	
	//}
	lightPow_ -= 0.001f;
	//SceneManager::GetInstance().SetPointLightPos(lightPow_);

	// 発射キー
	
	




}

void GameScene::Draw(void)
{
	stage_->Draw();
	enemy_->Draw();

	DrawSphere3D(circlePos_, CIRCLE_RADIUS, 10, 0xff0000, 0xff0000, true);
	DrawFormatString(
		0, 40, 0xFFFFFF, "球座標：(%.2f, %.2f, %.2f)",
		circlePos_.x, circlePos_.y, circlePos_.z);

	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();

	DrawFormatString(
		0, 80, 0xFFFFFF, "カメラ座標：(%.2f, %.2f, %.2f)",
		cameraPos.x, cameraPos.y, cameraPos.z);

}

void GameScene::Release(void)
{
	stage_->Release();
	delete stage_;
	stage_ = nullptr;

	enemy_->Release();
	delete enemy_;
	enemy_ = nullptr;
}

bool GameScene::CollisionCamera(VECTOR pos)
{
	auto& camera = SceneManager::GetInstance().GetCamera();

	// Camera の次の座標を取得
	VECTOR nextPos = camera.Move();

	// 衝突判定
	int stageModelId = stage_->GetGoalModelId();
	auto info = MV1CollCheck_Sphere(stageModelId, -1, nextPos, camera.CAMERA_RADIUS);
	if (info.HitNum != 0)
	{
		// 衝突なし → 座標を確定
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::CLEAR);
		return true;
	}

	stageModelId = stage_->GetModelId();
	info = MV1CollCheck_Sphere(stageModelId, -1, nextPos, camera.CAMERA_RADIUS);

	if (info.HitNum == 0)
	{
		// 衝突なし → 座標を確定
		camera.ApplyMove(nextPos);
		return true;
	}
	else
	{
		// 衝突あり → pos_ は更新されない
	}

	/*auto& camera = SceneManager::GetInstance().GetCamera();

	int stageModelId = stage_->GetModelId();
	
	auto info = MV1CollCheck_Sphere(
		stageModelId, -1, camera.GetPos(), camera.CAMERA_RADIUS);
	if (info.HitNum > 0)
	{
		camera.SetCollision(true);
		return true;
	}
	camera.SetCollision(false);
	return false;*/
	return false;
}

bool GameScene::Collision(void)
{
	// ステージモデルID
	int stageModelId = stage_->GetModelId();
	// 弾を取得する
	//std::vector<ShotBase*> shots = cannon_->GetShots();
	//for (ShotBase* shot : shots)
	{
		//if (!shot->IsCollisionState())
		//{
		//	// 爆発中や処理終了後は、以降の処理は実行しない
		//	continue;
		//}
		// ステージモデルとの衝突判定
		
	}
	auto info = MV1CollCheck_Sphere(
		stageModelId, -1, circlePos_, CIRCLE_RADIUS);
	if (info.HitNum > 0)
	{
		//shot->Blast();
		int a = 0;
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
		return true;

		//	// --- 壁・床などの全ての衝突ポリゴンを見る ---
		//	for (int i = 0; i < info.HitNum; i++)
		//	{
		//		auto& hit = info.Dim[i];
		//		// ポリゴンの中心を計算
		//		VECTOR hitPos = {
		//			(hit.Position[0].x + hit.Position[1].x + hit.Position[2].x) / 3.0f,
		//			(hit.Position[0].y + hit.Position[1].y + hit.Position[2].y) / 3.0f,
		//			(hit.Position[0].z + hit.Position[1].z + hit.Position[2].z) / 3.0f
		//		};
		//		VECTOR hitNormal = hit.Normal;

		//		// ----------- 壁との衝突（横方向のポリゴン） -----------
		//		// 床 / 天井 ではない場合 = 横の壁
		//		if (fabs(hitNormal.y) < 0.5f) // 横壁の法線はY成分が小さい
		//		{
		//			// めり込み防止：法線方向に押し戻す
		//			float pushBack = 2.0f; // 押し戻し係数（大きければ強く押し返す）
		//			circlePos_ = VAdd(circlePos_, VScale(hitNormal, pushBack));

		//			// 壁に当たったときの減速
		//			powdddd /= 1.5f;

		//			// ここで跳ね返しSEとかつけてもOK
		//		}
		//	}

		//	// バウンド
		//if (circlePos_.y - CIRCLE_RADIUS < 0.0f)
		//{
		//	auto& sceneMana = SceneManager::GetInstance();

		//	circlePos_.y = 0.0f + CIRCLE_RADIUS;

		//	powdddd /= 2.0f;
		//	pow = powdddd;
		//	sceneMana.SetPointLightPos(circlePos_);

		//	if (!isCollision_)
		//	{
		//		sceneMana.IsPointLightPow();
		//		isCollision_ = true;
		//	}
		//	
		//}
		//}
		// 当たり判定結果ポリゴン配列の後始末をする
		//MV1CollResultPolyDimTerminate(info);
	}
	return false;
}
