#include "ShotNormal.h"

ShotNormal::ShotNormal(TYPE type, int baseModelId)
	: ShotBase(type, baseModelId)
{
}

ShotNormal::~ShotNormal(void)
{
}

void ShotNormal::SetParam(void)
{
	// ’e‚Ì‘å‚«‚³‚ğİ’è
	scl_ = { 0.8f, 0.8f, 0.8f };

	// ’e‚ÌŠp“x‚ğİ’è
	rot_ = { 0.0f, 0.0f, 0.0f };

	// ’e‚Ì‘¬“x
	speed_ = 8.0f;

	// ’e‚Ì¶‘¶ŠúŠÔ
	cntAlive_ = 200;

	// ’e‚Ì”¼Œa
	collisionRadius_ = 10.0f;

}
