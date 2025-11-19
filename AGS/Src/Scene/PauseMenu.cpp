#include "PauseMenu.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
#include "../Application.h"

void PauseMenu::Update()
{
    if (!isActive_) return;

    auto& ins = InputManager::GetInstance();

    // パッド取得
    InputManager::JOYPAD_IN_STATE padState =
        ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

    auto trigger = ins.GetDirectionXZAKey(padState.AKeyLX, padState.AKeyLY);
    auto f = trigger.z;


    // ==========================================
    // ① 通常メニュー状態
    // ==========================================
    if (state_ == MENU_STATE::MAIN)
    {
        // --- 移動 ---
        if (ins.IsTrgDown(KEY_INPUT_UP) ||
            ins.IsTrgDown(KEY_INPUT_W) ||
            f > 0.0f)
        {
            if (!isT) selectedIndex_--;
            if (selectedIndex_ < 0) selectedIndex_ = static_cast<int>(items_.size()) - 1;
        }
        else if (ins.IsTrgDown(KEY_INPUT_DOWN) ||
            ins.IsTrgDown(KEY_INPUT_S) ||
            f < 0.0f)
        {
            if (!isT) selectedIndex_++;
            if (selectedIndex_ >= static_cast<int>(items_.size())) selectedIndex_ = 0;
        }

        // --- 決定 ---
        if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
            ins.IsTrgDown(KEY_INPUT_SPACE) ||
            ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
        {
            switch (selectedIndex_)
            {
            case 0: // 再開
                isActive_ = false;
                break;

            case 1: // タイトル
                SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
                break;

            case 2: // 終了 → 確認状態へ
                if (isConfirm_)
                {
                    isActive_ = false;
                    break;
                }
                state_ = MENU_STATE::CONFIRM;
                selectedIndex_ = 1; // 「いいえ」を初期位置
                break;
            }
        }
    }


    // ==========================================
    // ② 終了確認状態
    // ==========================================
    else if (state_ == MENU_STATE::CONFIRM)
    {
        // 左右移動（2択）
        if (ins.IsTrgDown(KEY_INPUT_LEFT) || ins.IsTrgDown(KEY_INPUT_A) || trigger.x < 0)
        {
            if (!isT) selectedIndex_ = 0; // はい
        }
        else if (ins.IsTrgDown(KEY_INPUT_RIGHT) || ins.IsTrgDown(KEY_INPUT_D) || trigger.x > 0)
        {
            if (!isT) selectedIndex_ = 1; // いいえ
        }

        // 決定
        if (ins.IsTrgDown(KEY_INPUT_RETURN) ||
            ins.IsTrgDown(KEY_INPUT_SPACE) ||
            ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
        {
            if (selectedIndex_ == 0)
            {
                // はい → 本当に終了
                Application::GetInstance().GameEnd();
            }
            else
            {
                // いいえ → メインメニューに戻る
                state_ = MENU_STATE::MAIN;
                selectedIndex_ = 0;
            }
        }
    }

    // トリガーフラグ更新
    isT = (trigger.x != 0.0f) || (trigger.y != 0.0f) || (trigger.z != 0.0f);
}

void PauseMenu::Draw()
{
    if (!isActive_) return;

    // 半透明背景
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
    DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), TRUE);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    DrawString(500, 150, "PAUSE", GetColor(255, 255, 255));


    // ==========================================
    // ① 通常メニューの描画
    // ==========================================
    if (state_ == MENU_STATE::MAIN)
    {
        int y = 300;
        for (int i = 0; i < items_.size(); i++)
        {
            const char* cursor = (i == selectedIndex_) ? "→ " : "   ";
            int color = (i == selectedIndex_) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);

            DrawFormatString(
                500,
                y + i * 50,
                color,
                "%s%s",
                cursor,
                items_[i].c_str()
            );
        }
    }


    // ==========================================
    // ② 終了確認ウィンドウの描画
    // ==========================================
    else if (state_ == MENU_STATE::CONFIRM)
    {
        DrawString(460, 260, "ゲームを終了しますか？", GetColor(255, 255, 255));

        int y = 350;

        // はい
        {
            const char* cursor = (selectedIndex_ == 0) ? "→ " : "   ";
            int color = (selectedIndex_ == 0) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
            DrawFormatString(430, y, color, "%sはい", cursor);
        }

        // いいえ
        {
            const char* cursor = (selectedIndex_ == 1) ? "→ " : "   ";
            int color = (selectedIndex_ == 1) ? GetColor(255, 255, 0) : GetColor(200, 200, 200);
            DrawFormatString(630, y, color, "%sいいえ", cursor);
        }
    }
}
