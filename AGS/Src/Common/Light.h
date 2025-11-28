#pragma once
#include <DxLib.h>

class Light
{
public:
    Light();
    ~Light();

    void Init();
    void Destroy();
    void Update(float deltaTime);

    void SetPosition(const VECTOR& pos);
    void SetActive(bool active);
    void SetPower(float power);

    bool IsActive() const { return isActive_; }
    float GetPower() const { return lightPow_; }
    const VECTOR& GetPosition() const { return pos_; }
    int GetHandle() const { return handle_; }

private:
    int handle_;
    VECTOR pos_;
    float lightPow_;   // 現在の光の強さ（SceneManagerで使っていたやつ）
    bool isActive_;    // 有効かどうか
};