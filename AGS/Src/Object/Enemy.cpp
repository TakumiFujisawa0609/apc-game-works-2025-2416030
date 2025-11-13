#include "Enemy.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"


Enemy::Enemy(void)
	:
	animationController_(nullptr),
	modelId_(-1)
{
}

Enemy::~Enemy(void)
{
}

void Enemy::Init(void)
{
	modelId_ = MV1LoadModel((Application::PATH_MODEL + "Enemy/BlueDemon.mv1").c_str());

	MV1SetWireFrameDrawFlag(modelId_, TRUE);

	// アニメーションの設定
	animationController_ = new AnimationController(modelId_);
	for (int i = 0; i < static_cast<int>(ANIM_TYPE::MAX); i++)
	{
		animationController_->AddInFbx(i, 30.0f, i);
	}

	MV1SetPosition(modelId_, { 3060.0f, 0.0f, 5000.0f });
	MV1SetRotationXYZ(modelId_, { 0.0f, 0.0f, 0.0f });
}

void Enemy::Update(void)
{
	Move();
}

void Enemy::Draw(void)
{
	//MV1SetMaterialOutLineWidth(modelId_, 2.0f);   // 輪郭線を太めに
	//MV1SetMaterialOutLineColor(modelId_, GetColor(255, 0, 0));  // 赤い縁取りなど
	//MV1SetUseOrigShader(TRUE);  // モデルの陰影をオフ
	MV1DrawModel(modelId_);
}

void Enemy::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);
}

VECTOR Enemy::GetHeadPos(void)
{
	auto headFrame = MV1SearchFrame(modelId_, "Head");
	auto headPos = MV1GetFramePosition(modelId_, headFrame);
	return headPos;
}

void Enemy::Move()
{
	// デルタタイム
	float dt = SceneManager::GetInstance().GetDeltaTime();
	const float stopDistance = 2.0f;
	const float moveSpeed = 5.0f;

	VECTOR diff;

	//--------------------------------
	// targetPos_ が設定されている場合はそちらへ
	//--------------------------------
	if (isMoveSpot_)
	{
		// yは無視（平面）
		diff = VSub(targetPos_, pos_);
		diff.y = 0.0f;
		float dist = VSize(diff);

		if (dist > stopDistance)
		{
			VECTOR moveVec = VScale(VNorm(diff), moveSpeed * dt);
			pos_ = VAdd(pos_, moveVec);
		}
		else
		{
			// 到着
			isMoveSpot_ = false;
		}
	}
	else
	{
		//--------------------------------
		// ランダム移動（自動徘徊）
		//--------------------------------
		static const VECTOR moveSpots[] = { SPOT1_POS, SPOT2_POS, SPOT3_POS };

		int r = GetRand(_countof(moveSpots) - 1);
		targetPos_ = moveSpots[r];
		targetPos_.y = 0.0f;

		isMoveSpot_ = true;
		return; // 次フレームから追い始める
	}

	//--------------------------------
	// 回転処理（ターゲット方向を向く）
	//--------------------------------
	float angleY = atan2f(diff.x, diff.z);

	// モデルが反対向きなら補正
	const float angleOffset = DX_PI_F;
	float finalAngleY = angleY + angleOffset;

	MATRIX rotMat = MGetRotY(finalAngleY);
	rotMat.m[3][0] = pos_.x;
	rotMat.m[3][1] = pos_.y;
	rotMat.m[3][2] = pos_.z;
	MV1SetMatrix(modelId_, rotMat);
}

