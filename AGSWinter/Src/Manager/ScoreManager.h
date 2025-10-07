#pragma once
#include <vector>

class ScoreManager {
public:
    // スコアの追加
    static void AddScore(int score);

    // スコアの取得（const参照で安全）
    static const std::vector<int>& GetScores();

    static const int GetScoreLength();

    // スコアのリセット
    static void ClearScores();

private:
    // スコアを保持する静的配列
    static std::vector<int> scores;
};