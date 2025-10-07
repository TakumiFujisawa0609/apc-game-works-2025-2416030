#include <DxLib.h>
#include "Camera.h"
#include "../Utility/AsoUtility.h"

Camera::Camera()
{
}
Camera::~Camera()
{
}


void Camera::Init()
{
	pos_ = INIT_CAMERA_POS;
	angles_ = { 0.0f,0.0f,0.0f };
}

void Camera::Update()
{
    VECTOR moveDir = AsoUtility::VECTOR_ZERO;

    // 同時押しも反映できるように加算
    if (CheckHitKey(KEY_INPUT_I)) { moveDir = VAdd(moveDir, AsoUtility::DIR_F); }
    if (CheckHitKey(KEY_INPUT_K)) { moveDir = VAdd(moveDir, AsoUtility::DIR_B); }
    if (CheckHitKey(KEY_INPUT_J)) { moveDir = VAdd(moveDir, AsoUtility::DIR_L); }
    if (CheckHitKey(KEY_INPUT_L)) { moveDir = VAdd(moveDir, AsoUtility::DIR_R); }

    // 方向が入力されていたら
    if (VSize(moveDir) > 0.0f) {
        moveDir = VNorm(moveDir); // 斜め移動を等速にする

        // カメラの向き（angles_）を反映する回転行列を作る
        MATRIX rot = MGetIdent();
        rot = MMult(rot, MGetRotY(angles_.y)); // Y軸（左右回転）
        rot = MMult(rot, MGetRotX(angles_.x)); // X軸（上下回転）

        // 移動方向をカメラ座標系に変換
        moveDir = VTransform(moveDir, rot);

        // 座標を更新
        pos_ = VAdd(pos_, VScale(moveDir, SPEED_MOVE));
    }

    // 回転処理
    float anglePowRad = AsoUtility::Deg2RadF(SPEED_ANGLE_DEG);
    if (CheckHitKey(KEY_INPUT_U)) { angles_.x += anglePowRad; } // 上下
    if (CheckHitKey(KEY_INPUT_P)) { angles_.x -= anglePowRad; }
    if (CheckHitKey(KEY_INPUT_H)) { angles_.y += anglePowRad; } // 左右
    if (CheckHitKey(KEY_INPUT_SEMICOLON)) { angles_.y -= anglePowRad; }
}



void Camera::SetCameraPos()
{
	SetCameraPositionAndAngle(pos_, angles_.x, angles_.y, angles_.z);
}