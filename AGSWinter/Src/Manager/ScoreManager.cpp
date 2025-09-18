#include "ScoreManager.h"

// Ã“Iƒƒ“ƒo‚Ì’è‹`
std::vector<int> ScoreManager::scores;

void ScoreManager::AddScore(int score)
{
    scores.push_back(score);
}

const std::vector<int>& ScoreManager::GetScores() 
{
    return scores;
}

const int ScoreManager::GetScoreLength()
{
    return scores.size();
}

void ScoreManager::ClearScores()
{
    scores.clear();
}