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

    // 停止距離（この距離より近づいたら止まる）
    const float stopDistance = 2.0f; // ← 好きな値に調整（例：2.0f = 2m 手前）

    // DeltaTime取得
    float dt = SceneManager::GetInstance().GetDeltaTime();

    // 方向ベクトル（XZ平面）
    VECTOR diff = VSub(targetPos, pos_);
    diff.y = 0.0f;
    float dist = VSize(diff);

    // stopDistance より遠いときだけ移動
    if (dist > stopDistance)
    {
        // 実際に進む距離
        float moveDist = moveSpeed * dt;

        // 距離を超えないように調整
        if (moveDist > dist - stopDistance) moveDist = dist - stopDistance;

        // 正規化して移動ベクトル作成
        VECTOR moveVec = VScale(VNorm(diff), moveDist);

        // 位置更新
        pos_ = VAdd(pos_, moveVec);
    }

    // --- 回転処理 ---
    float angleY = atan2f(diff.x, diff.z);

    // モデルの前方向が180度反対だった場合
    const float angleOffset = DX_PI_F;
    float finalAngleY = angleY + angleOffset;

    MATRIX rotMat = MGetRotY(finalAngleY);

    // 位置も反映
    rotMat.m[3][0] = pos_.x;
    rotMat.m[3][1] = pos_.y;
    rotMat.m[3][2] = pos_.z;

    // モデルに反映
    MV1SetMatrix(modelId_, rotMat);
}
