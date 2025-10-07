#pragma once
#include <DxLib.h>
#include "../Application.h"
#include "SceneBase.h"

class SceneManager;
class Stage;

class GameScene : public SceneBase
{
public:
	constexpr static VECTOR GAMEOVER_POS = { 450.0f, 30.0f, 75.0f };
	constexpr static float GAMEOVER_COLLISION_RADIUS = 35.0f;

	constexpr static int NEXTWAVE_SIZE_WID = 624;
	constexpr static VECTOR NEXTWAVE_INIT_POS = { Application::SCREEN_SIZE_X + NEXTWAVE_SIZE_WID / 2, Application::SCREEN_SIZE_Y / 2, 0.0f };
	constexpr static float NEXTWAVE_MOVE_SPEED = 10.0f;
	constexpr static float NEXTWAVE_STOP_TIME = 1.0f;

	constexpr static float GRAVITY = 0.5f;



	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	// ステージインスタンス
	Stage* stage_; 

	VECTOR circlePos_ = { 0,0,0 };
	float pow = 20.0f;
	float powdddd = 20.0f;

	float lightPow_ = 0.0f;

	// 移動方向
	VECTOR moveDir_;

	VECTOR movePow;

	bool isShooting = false;

	VECTOR startPos_;  // 発射開始位置
	float maxDistance = 1000.0f;  // これ以上飛んだら停止

	void Collision(void);
	void CollisionGameOver();
};