#include "PauseMenu.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"

void PauseMenu::Update()
{
    if (!isActive_) return;

    auto& ins = InputManager::GetInstance();

    // 接続されているゲームパッド１の情報を取得
    InputManager::JOYPAD_IN_STATE padState =
        ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    auto trigger = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);

    auto f = trigger.z;


    // --- メニュー移動 ---
    if (ins.IsTrgDown(KEY_INPUT_UP) ||
        f > 0.0f)
    {
        if(!isT)
        {
            selectedIndex_--;
        }
        if (selectedIndex_ < 0) selectedIndex_ = static_cast<int>(items_.size()) - 1;
    }
    else if (ins.IsTrgDown(KEY_INPUT_DOWN) ||
        f < 0.0f
        )
    {
        if (!isT)
        {
            selectedIndex_++;
        }
        if (selectedIndex_ >= static_cast<int>(items_.size())) selectedIndex_ = 0;
    }

    // --- 決定キー処理 ---
    if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
        ins.IsTrgDown(KEY_INPUT_SPACE) ||
        ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN)) // 決定ボタン
    {
        switch (selectedIndex_)
        {
        case 0: // 再開
            isActive_ = false;
            break;

        case 1: // タイトルに戻る
            SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
            break;

        case 2: // 終了
            Application::GetInstance().GameEnd();
            break;
        }
    }
    isT = (trigger.x != AsoUtility::VECTOR_ZERO.x) ||
        (trigger.y != AsoUtility::VECTOR_ZERO.y) ||
        (trigger.z != AsoUtility::VECTOR_ZERO.z);
}

void PauseMenu::Draw()
{
    if (!isActive_) return;

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawString(500, 150, "PAUSE", GetColor(255, 255, 255));

    int y = 300;
    for (int i = 0; i < items_.size(); i++)
    {
        int color = (i == selectedIndex_) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
        DrawFormatString(500, y + i * 50, color, "%s", items_[i].c_str());
    }
}