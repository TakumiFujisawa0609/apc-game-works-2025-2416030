#pragma once
#include <DxLib.h>
#include <vector>
#include <string>

class PauseMenu
{
public:
    PauseMenu()
        : isActive_(false), selectedIndex_(0)
    {
        items_ = { "再開", "タイトルに戻る", "終了" };
        isT = false;
    }

    void Toggle() { isActive_ = !isActive_; }
    bool IsActive() const { return isActive_; }

    // 入力処理
    void Update();

    // 描画処理（ゲーム画面の上に重ねる）
    void Draw();

    // 現在選択中のメニューを取得
    int GetSelectedIndex() const { return selectedIndex_; }

private:
    bool isActive_;
    int selectedIndex_;
    std::vector<std::string> items_;
    bool isT;

    // 入力の押しっぱなし防止
    void WaitKeyInput();
};
