#pragma once
#include <DxLib.h>
#include <vector>
#include <string>

class PauseMenu
{
public:
    enum class MENU_STATE {
        MAIN,       // pause 通常メニュー
        CONFIRM     // 終了確認
    };

    PauseMenu()
        : isActive_(false), selectedIndex_(0)
    {
        items_ = { "再開", "タイトルに戻る", "終了" };
        isT = false;
    }

    void Toggle() { isActive_ = !isActive_; }
    void GameExitConfirm() { state_ = MENU_STATE::CONFIRM; isConfirm_ = true; isActive_ = !isActive_; }
    bool IsActive() const { return isActive_; }

    // 入力処理
    void Update();

    // 描画処理（ゲーム画面の上に重ねる）
    void Draw();

    // 現在選択中のメニューを取得
    int GetSelectedIndex() const { return selectedIndex_; }

private:

    MENU_STATE state_ = MENU_STATE::MAIN;
    bool isConfirm_ = false;

    bool isActive_;
    int selectedIndex_;
    std::vector<std::string> items_;
    bool isT;
};
