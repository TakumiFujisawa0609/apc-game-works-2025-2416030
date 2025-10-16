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

	targetPos_ = camera.GetLightPos();
	//pos_.y = 0.0f;
	MV1SetPosition(modelId_, pos_);


}
