#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "SceneManager.h"
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

VECTOR Camera::Move()
{
	// カメラ角度の回転処理（上下左右）
	float anglePowRad = AsoUtility::Deg2RadF(SPEED_ANGLE_DEG);
	if (CheckHitKey(KEY_INPUT_DOWN)) angles_.x += anglePowRad;
	if (CheckHitKey(KEY_INPUT_UP))   angles_.x -= anglePowRad;
	if (CheckHitKey(KEY_INPUT_RIGHT)) angles_.y += anglePowRad;
	if (CheckHitKey(KEY_INPUT_LEFT))  angles_.y -= anglePowRad;

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	// キーボード入力
	if (CheckHitKey(KEY_INPUT_W)) moveDir = VAdd(moveDir, AsoUtility::DIR_F);
	if (CheckHitKey(KEY_INPUT_S)) moveDir = VAdd(moveDir, AsoUtility::DIR_B);
	if (CheckHitKey(KEY_INPUT_A)) moveDir = VAdd(moveDir, AsoUtility::DIR_L);
	if (CheckHitKey(KEY_INPUT_D)) moveDir = VAdd(moveDir, AsoUtility::DIR_R);

	// 入力があれば正規化
	if (VSize(moveDir) <= 0.0f) return pos_;

	moveDir = VNorm(moveDir);

	// カメラ角度を反映した回転行列
	MATRIX rot = MGetRotY(angles_.y);
	moveDir = VTransform(moveDir, rot);

	// 次の座標を計算
	VECTOR nextPos = VAdd(pos_, VScale(moveDir, SPEED_MOVE * SceneManager::GetInstance().GetDeltaTime()));

	return nextPos; // 更新はここではしない
}

void Camera::ApplyMove(const VECTOR& newPos)
{
	pos_ = newPos; // 確定的に座標を反映
}


void Camera::SetBeforeDrawFixedPoint(void)
{
}

void Camera::SetBeforeDrawFree(void)
{
	auto& ins = InputManager::GetInstance();

	VECTOR moveDir = AsoUtility::VECTOR_ZERO;

	if (GetJoypadNum() == 0)
	{
		// 方向回転によるXYZの移動
		// 同時押しも反映できるように加算
		if (CheckHitKey(KEY_INPUT_W)) { moveDir = VAdd(moveDir, AsoUtility::DIR_F); }
		if (CheckHitKey(KEY_INPUT_S)) { moveDir = VAdd(moveDir, AsoUtility::DIR_B); }
		if (CheckHitKey(KEY_INPUT_A)) { moveDir = VAdd(moveDir, AsoUtility::DIR_L); }
		if (CheckHitKey(KEY_INPUT_D)) { moveDir = VAdd(moveDir, AsoUtility::DIR_R); }
	}
	else
	{
		float rotPow = 1.0f * DX_PI_F / 180.0f;

		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

		// アナログキーの入力値から方向を取得
		VECTOR dir = ins.GetDirectionXZAKey(padState.AKeyRX, padState.AKeyRY);
		VECTOR dir2 = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
	

		// 右スティック上下の傾き
		angles_.x -= dir.z * rotPow * 2.0f;

		// 右スティック上下の傾き
		angles_.y += dir.x * rotPow * 2.0f;

		if (!AsoUtility::EqualsVZero(dir2))
		{
			// XYZの回転行列
			// XZ平面移動にする場合は、XZの回転を考慮しないようにする
			MATRIX mat = MGetIdent();
			//mat = MMult(mat, MGetRotX(angles_.x));

			mat = MMult(mat, MGetRotY(angles_.y));
			//mat = MMult(mat, MGetRotZ(angles_.z));

			// 回転行列を使用して、ベクトルを回転させる
			moveDir_ = VTransform(dir2, mat);

			// 移動方向から角度に変換する
			//angles_.y = atan2f(moveDir_.x, moveDir_.z);

			// 方向×スピードで移動量を作って、座標に足して移動
			pos_ = VAdd(pos_, VScale(moveDir_, SPEED_MOVE));
		}
	}
		// 接続されているゲームパッド１の情報を取得
		InputManager::JOYPAD_IN_STATE padState =
			ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
		// アナログキーの入力値から方向を取得
		
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
