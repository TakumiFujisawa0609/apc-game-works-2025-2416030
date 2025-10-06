#include <EffekseerForDXLib.h>
#include "../../Manager/SceneManager.h"
#include "ShotBase.h"

ShotBase::ShotBase(TYPE type, int baseModelId)
{
	// 使用メモリ容量と読み込み時間の削減のため
 // モデルデータをいくつもメモリ上に存在させない
 // CreateShotで処理してしまうと、再利用する度にモデルが複製され、
 // Releaseもされない状況になってしまう
	modelId_ = MV1DuplicateModel(baseModelId);

	type_ = type;
}

ShotBase::~ShotBase(void)
{
}

void ShotBase::CreateShot(VECTOR pos, VECTOR dir)
{
	// 弾の発射位置を設定
	pos_ = pos;

	// 弾の発射方向の設定
	dir_ = dir;

	// 重力
	gravityPow_ = 0.0f;

	state_ = STATE::SHOT;

	// パラメータ設定
	SetParam();

	// 大きさの設定
	MV1SetScale(modelId_, scl_);

	// 回転の設定
	MV1SetRotationXYZ(modelId_, rot_);

	// 位置の設定
	MV1SetPosition(modelId_, pos_);
}

void ShotBase::Update(void)
{
	if (!IsAlive())
	{
		// 生存していなければ処理中断
		return;
	}

	switch (state_)
	{
	case ShotBase::STATE::SHOT:
		UpdateShot();
		break;
	case ShotBase::STATE::BLAST:
		UpdateBlast();
		break;
	case ShotBase::STATE::END:
		UpdateEnd();
		break;
	}
}

void ShotBase::Draw(void)
{
	if (!IsAlive())
	{
		// 生存していなければ処理中断
		return;
	}

	switch (state_)
	{
	case ShotBase::STATE::SHOT:
		DrawShot();
		break;
	case ShotBase::STATE::BLAST:
		DrawBlast();
		break;
	case ShotBase::STATE::END:
		DrawEnd();
		break;
	}
}

void ShotBase::Release(void)
{
	MV1DeleteModel(modelId_);
}

void ShotBase::ChangeState(STATE state)
{
	if (state_ == state)
	{
		return; // 既に同じ状態なら何もしない
	}
	// 状態を変更
	state_ = state;
	switch (state_)
	{
	case ShotBase::STATE::SHOT:
		ChangeShot();
		break;
	case ShotBase::STATE::BLAST:
		ChangeBlast();
		break;
	case ShotBase::STATE::END:
		ChangeEnd();
		break;
	}
}

void ShotBase::Blast(EFFECT_TYPE type)
{
	effectType_ = type;
	ChangeState(STATE::BLAST);
}

void ShotBase::ReduceCntAlive(void)
{
	cntAlive_--;
	if (cntAlive_ < 0)
	{
		ChangeState(STATE::END);
	}
}

void ShotBase::ChangeShot(void)
{
}

void ShotBase::ChangeBlast(void)
{
}

void ShotBase::ChangeEnd(void)
{
}

void ShotBase::UpdateBlast(void)
{
}

void ShotBase::UpdateShot(void)
{
	// 弾を移動させる
	pos_ = VAdd(pos_, VScale(dir_, speed_));

	// 加速度的に重力を加える
	gravityPow_ +=
		SceneManager::GRAVITY * SceneManager::GetInstance().GetDeltaTime();

	pos_.y -= gravityPow_;

	// 大きさの設定
	MV1SetScale(modelId_, scl_);

	// 角度の設定
	MV1SetRotationXYZ(modelId_, rot_);

	// 位置の設定
	MV1SetPosition(modelId_, pos_);

	// 生存カウンタの減少
	ReduceCntAlive();
}

void ShotBase::UpdateEnd(void)
{
}

void ShotBase::DrawBlast(void)
{
}

void ShotBase::DrawShot(void)
{
	MV1DrawModel(modelId_);

#ifdef _DEBUG
	// デバッグ用：衝突判定用球体
	DrawSphere3D(pos_, collisionRadius_, 10, 0x0000ff, 0x0000ff, false);
#endif // _DEBUG
}

void ShotBase::DrawEnd(void)
{
}
