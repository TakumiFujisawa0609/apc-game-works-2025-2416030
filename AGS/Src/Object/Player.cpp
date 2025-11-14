#include <DxLib.h>
#include "../Manager/Camera.h"
#include "Player.h"

Player::Player(void)
	:
	camera_(nullptr)
{
}

Player::~Player(void)
{
}

void Player::Init(void)
{
	camera_ = new Camera();
}

void Player::Update(void)
{
	// ƒJƒƒ‰‚ÌƒAƒ“ƒOƒ‹‚ðŽæ“¾
	auto& cameraAngle_ = camera_->GetAngle();

	MATRIX mat = MGetIdent();
}

void Player::Draw(void)
{
}

void Player::Release(void)
{
}
