#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "Camera.h"

Camera::Camera(void)
{
}

Camera::~Camera(void)
{
}


// 初期化
void Camera::Init(void)
{
	pos_ = DEFAULT_POS;
	angles_ = DEFAULT_ANGLES;

	farClip_ = CAMERA_FAR;
	farClip_ = 100.0f;
}

// 更新
void Camera::Update(void)
{

}

// 描画前のカメラ設定
void Camera::SetBeforeDraw(void)
{
	// クリップ距離を設定
	SetCameraNearFar(CAMERA_NEAR, farClip_);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	}

	// カメラの設定(位置と角度による制御)
	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();
}

// デバッグ用描画
void Camera::DrawDebug(void)
{
	DrawFormatString(
		0, 0, 0x000000, "カメラ座標：(%.2f, %.2f, %.2f)",
		pos_.x, pos_.y, pos_.z);
	DrawFormatString(
		0, 20, 0x000000, "カメラ角度：(%.2f, %.2f, %.2f)",
		angles_.x * 180.0f / DX_PI_F,
		angles_.y * 180.0f / DX_PI_F,
		angles_.z * 180.0f / DX_PI_F);

}

// 解放
void Camera::Release(void)
{
}

void Camera::ChangeMode(MODE mode)
{
	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	// 同時押しも反映できるように加算
	if (CheckHitKey(KEY_INPUT_W)) { moveDir = VAdd(moveDir, AsoUtility::DIR_F); }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = VAdd(moveDir, AsoUtility::DIR_B); }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = VAdd(moveDir, AsoUtility::DIR_L); }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = VAdd(moveDir, AsoUtility::DIR_R); }

	// 方向が入力されていたら
	if (VSize(moveDir) > 0.0f) {
		moveDir = VNorm(moveDir); // 斜め移動を等速にする

		// カメラの向き（angles_）を反映する回転行列を作る
		MATRIX rot = MGetIdent();
		rot = MMult(rot, MGetRotY(angles_.y)); // Y軸（左右回転）
		//rot = MMult(rot, MGetRotX(angles_.x)); // X軸（上下回転）

		// 移動方向をカメラ座標系に変換
		moveDir = VTransform(moveDir, rot);

		if(!isCollision_)
		{
			// 座標を更新
			pos_ = VAdd(pos_, VScale(moveDir, SPEED_MOVE));
		}
		else
		{

		}
	}

	// 回転処理
	float anglePowRad = AsoUtility::Deg2RadF(SPEED_ANGLE_DEG);
	if (CheckHitKey(KEY_INPUT_DOWN)) { angles_.x += anglePowRad; } // 上下
	if (CheckHitKey(KEY_INPUT_UP)) { angles_.x -= anglePowRad; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { angles_.y += anglePowRad; } // 左右
	if (CheckHitKey(KEY_INPUT_LEFT)) { angles_.y -= anglePowRad; }


	else
	{
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		//dir = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
	}
	//// WASDでカメラの位置を変える
	//float movePow = 3.0f;

	//VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	//if (ins.IsNew(KEY_INPUT_W)) { pos_.z += movePow; moveDir = AsoUtility::DIR_U; }
	//if (ins.IsNew(KEY_INPUT_A)) { pos_.x -= movePow; moveDir = AsoUtility::DIR_L; }
	//if (ins.IsNew(KEY_INPUT_S)) { pos_.z -= movePow; moveDir = AsoUtility::DIR_D; }
	//if (ins.IsNew(KEY_INPUT_D)) { pos_.x += movePow; moveDir = AsoUtility::DIR_R; }
	//if (ins.IsNew(KEY_INPUT_Q)) { pos_.y += movePow; }
	//if (ins.IsNew(KEY_INPUT_E)) { pos_.y -= movePow; }

	//MATRIX mat = MGetIdent();

	//mat = MMult(mat, MGetRotX(moveDir.x));

	//// 矢印キーでカメラの角度を変える
	//float rotPow = 1.0f * DX_PI_F / 180.0f;
	//if (CheckHitKey(KEY_INPUT_DOWN)) { angles_.x += rotPow; }
	//if (CheckHitKey(KEY_INPUT_UP)) { angles_.x -= rotPow; }
	//if (CheckHitKey(KEY_INPUT_RIGHT)) { angles_.y += rotPow; }
	//if (CheckHitKey(KEY_INPUT_LEFT)) { angles_.y -= rotPow; }
}
