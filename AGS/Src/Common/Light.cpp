#include "Light.h"

Light::Light()
{
    handle_ = -1;
    pos_ = VGet(0, 0, 0);
    lightPow_ = 0.0f;
    isActive_ = false;
}

Light::~Light()
{
    Destroy();
}

void Light::Init()
{
    VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

    // 元の標準ポイントライトと同じパラメータ
    handle_ = CreatePointLightHandle(
        pos,
        400.0f,            // Range
        0.000001f,         // Atten0
        0.01f,             // Atten1
        0.0000001f         // Atten2
    );

    SetLightEnableHandle(handle_, TRUE);
}

void Light::Destroy()
{
    if (handle_ != -1)
    {
        DeleteLightHandle(handle_);
        handle_ = -1;
    }
}

void Light::Update(float deltaTime)
{
    //if (!isActive_) return;

    // SceneManagerでやっていた lightPow の上昇処理をそのまま移行
    lightPow_ += 0.5f * deltaTime;

    if (lightPow_ > 1.0f)
    {
        lightPow_ = 1.0f;
        isActive_ = false;
    }

    // DxLib に反映
    SetLightPositionHandle(handle_, pos_);
    SetLightRangeAttenHandle(handle_,400.0f, 0.000001f, lightPow_, 0.0000001f);
    SetLightDifColorHandle(handle_, GetColorF(lightPow_, lightPow_, lightPow_, 1.0f));
}

void Light::SetPosition(const VECTOR& pos)
{
    pos_ = pos;
}

void Light::SetActive(bool active)
{
    isActive_ = active;
    if (active)
    {
        lightPow_ = 0.0f;
    }
}

void Light::SetPower(float power)
{
    lightPow_ = power;
}
