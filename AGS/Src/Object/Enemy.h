#pragma once

#include <DxLib.h>
class AnimationController;

class Enemy
{
public:
	// アニメーション種別
	enum class ANIM_TYPE
	{
		DEATH,
		DUCK,
		HIT_REACT,
		IDLE,
		JUMP,
		JUMP_IDLE,
		JUMP_LAND,
		NO,
		PUNCH,
		RUN,
		WALK,
		WAVE,
		WEAPON,
		YES,
		MAX,
	};

	enum class MOVE_TYPE
	{

	};

	static constexpr float SPEED = 2.0f;

	// コンストラクタ
	Enemy(void);
	// デストラクタ
	~Enemy(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	void SetTargetPos(VECTOR pos) { targetPos_ = pos; }
private:
	// アニメーションコントローラ
	AnimationController* animationController_;

	// モデルのハンドルID
	int modelId_;

	// 座標
	VECTOR pos_;

	// 角度
	VECTOR angles_;

	// 大きさ
	VECTOR scales_;

	// 移動方向
	VECTOR moveDir_;

	// 移動速度
	float speed_;

	VECTOR targetPos_;

	void Move();
};

