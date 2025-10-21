#include "Enemy.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "Common/AnimationController.h"


Enemy::Enemy(void)
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
	MV1DrawModel(modelId_);
}

void Enemy::Release(void)
{
	// ロードされた３Ｄモデルをメモリから解放
	MV1DeleteModel(modelId_);
}

void Enemy::Move()
{
    auto& camera = SceneManager::GetInstance();

    // ターゲット位置（ライト）
    VECTOR targetPos = camera.GetLightPos();
    targetPos.y = 0.0f; // 高さ固定

    // 移動速度
    float moveSpeed = 5.0f;

    // DeltaTime取得
    float dt = SceneManager::GetInstance().GetDeltaTime();

    // 方向ベクトル
    VECTOR diff = VSub(targetPos, pos_);
    float dist = VSize(diff);

    if (dist > 0.01f)
    {
        // 正規化して移動量
        VECTOR moveVec = VScale(VNorm(diff), moveSpeed * dt);

        // 距離を超えないように調整
        if (VSize(moveVec) > dist) moveVec = diff;

        // 位置更新
        pos_ = VAdd(pos_, moveVec);
    }

    // Y軸回転を計算（XZ平面のみ）
    float angleY = atan2f(diff.x, diff.z); // ラジアン
    MATRIX rotMat = MGetRotY(angleY);

    // モデルに反映
    MV1SetMatrix(modelId_, rotMat);
    MV1SetPosition(modelId_, pos_);
}


