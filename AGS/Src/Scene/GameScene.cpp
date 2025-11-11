
#include "../Utility/AsoUtility.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Object/Stage.h"
#include "../Object/Enemy.h"
#include "../Manager/Camera.h"
#include "GameScene.h"
#include <algorithm>

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

	pauseMenu_ = new PauseMenu();

	isMove = false;

	rePress = newPress = 0;
}

void GameScene::Update(void)
{
	auto& ins = InputManager::GetInstance();

	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	rePress = newPress;
	newPress = ins.IsNew(KEY_INPUT_P);

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::TOP))
	{
		printfDx("Pad TOP Trigger\n");
	}
	if (ins.IsTrgDown(KEY_INPUT_UP))
	{
		printfDx("Keyboard UP Trigger\n");
	}

	// Pキーでポーズ切り替え
	if ((rePress == 0 && newPress == 1))
	{
		pauseMenu_->Toggle();
	}

	if (pauseMenu_->IsActive())
	{
		pauseMenu_->Update();
		return; // ポーズ中はゲーム更新停止
	}

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
			// 接地
			sceneMana.SetPointLightPos(circlePos_);
			sceneMana.IsPointLightPow();
			enemy_->SetTargetPos(circlePos_);
			isCollision_ = true;
			SceneManager::GetInstance().GetCamera().SetFarClip(1800.0f);
			PlaySoundMem(seId_, DX_PLAYTYPE_BACK);
			isMove = false;
		}
	}
	else
	{
		// 動く

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
		circlePos_ = VAdd(circlePos_, VScale(moveDir_, powdddd * SceneManager::GetInstance().GetDeltaTime()));

		circlePos_.x += movePow.x * SceneManager::GetInstance().GetDeltaTime();
		pow -= GRAVITY * SceneManager::GetInstance().GetDeltaTime();

		circlePos_.y += pow * SceneManager::GetInstance().GetDeltaTime();

		isMove = true;
	}
	

	// 発射キー
	if ((CheckHitKey(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
		&& !isMove)
	{
		isCollision_ = false;
		isShooting = true;
		startPos_ = cameraPos;

		MATRIX mat = MGetIdent();
		//mat = MMult(mat, MGetRotY(cameraAngle.y));
		//moveDir_ = VTransform({ 0.0f,0.0f,5.0f }, mat);


		mat = MGetIdent();
		mat = MMult(mat, MGetRotX(cameraAngle.x * 0.5f));
		mat = MMult(mat, MGetRotY(cameraAngle.y));
		//moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);
		moveDir_ = VTransform({ 0.0f,0.0f,1.0f }, mat);

		circlePos_ = VAdd(cameraPos, moveDir_);
		//isMove += powdddd;

		pow = 00.0f;
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
	lightPow_ -= 0.001f * SceneManager::GetInstance().GetDeltaTime();
	//SceneManager::GetInstance().SetPointLightPos(lightPow_);

	// 発射キー
	
	
	Collision();

	if (isMove)
	{
		auto& camera = SceneManager::GetInstance().GetCamera();

		// 投げた位置から今の球までの距離を算出
		float dist = VSize(VSub(circlePos_, startPos_));

		// FarClipを距離に応じて設定（最小値を確保）
		camera.SetFarClip(dist);
	}
}

void GameScene::Draw(void)
{
	stage_->Draw();

	if(isMove)
	{
		// ライトを一時的に無効化
		SetUseLighting(FALSE);
		// 球を描画（常に明るく見える）
		DrawSphere3D(circlePos_, CIRCLE_RADIUS, 10, 0xff0000, 0xffffff, TRUE);
		// ライトを元に戻す
		SetUseLighting(TRUE);
	}
	auto cameraPos = SceneManager::GetInstance().GetCamera().GetPos();
#ifdef DEBUG
	DrawFormatString(
		0, 40, 0xFFFFFF, "球座標：(%.2f, %.2f, %.2f)",
		circlePos_.x, circlePos_.y, circlePos_.z);

	DrawFormatString(
		0, 80, 0xFFFFFF, "カメラ座標：(%.2f, %.2f, %.2f)",
		cameraPos.x, cameraPos.y, cameraPos.z);
#endif // DEBUG

	

	//auto iu = ConvWorldPosToScreenPos(enemy_->GetPos());

	/*DrawFormatString(
		0, 120, 0xFFFFFF, "敵座標：(%.2f, %.2f, %.2f)",
		iu.x, iu.y, iu.z);
	DrawCircle(iu.x, iu.y - 100.0f, 20.0f, GetColor(255, 255, 255), true);*/

	auto ePos = enemy_->GetPos();
	VECTOR diff = VSub(ePos, cameraPos);
	auto a = VSize(diff);

	//if (CheckCameraViewClip(ePos)) return;
	//SceneManager::GetInstance().GetCamera().SetFarClip(moveNum);
	enemy_->Draw();
	SetUseZBuffer3D(TRUE);
	

	//if (a < 1000.0f) {
	//	// 近いときの処理
	//	auto sd = enemy_->GetHeadPos();
	//	float baseScale = 1.0f;          // 元のサイズ
	//	float maxDistance = 500.0f;      // スケール変化させたい最大距離

	//	float t = 1.0f - (a / maxDistance);
	//	t = std::clamp(t, 0.2f, 1.5f);   // 下限0.2倍〜上限1.5倍など制限

	//	float scale = baseScale * t;

	//	auto iu = ConvWorldPosToScreenPos(sd);
	//	DrawFormatString(0, 120, 0xFFFFFF, "敵座標：(%.2f, %.2f, %.2f)", iu.x, iu.y, iu.z);
	//	DrawCircle(iu.x, iu.y, 20.0f, GetColor(255, 0, 0), true);
	//}

	pauseMenu_->Draw();

	auto iu = ConvWorldPosToScreenPos(circlePos_);
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
	auto cPos = SceneManager::GetInstance().GetCamera().GetPos();
	cPos.y -= 300.0f;
	if (AsoUtility::IsHitSpheres(cPos, 20.0f, enemy_->GetPos(), 20.0f))
	{
		int a = 0;
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAMEOVER);
	}

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
