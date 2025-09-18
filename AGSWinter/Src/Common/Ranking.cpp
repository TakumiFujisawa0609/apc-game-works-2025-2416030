#include <DxLib.h>
#include "Ranking.h"

Ranking* Ranking::instance_ = nullptr;

Ranking::Ranking()
{
}


void Ranking::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Ranking();
	}
	instance_->Init();
}

Ranking& Ranking::GetInstance(void)
{
	return *instance_;
}

void Ranking::Init()
{
	// ハイスコアの初期化
	for (int ii = 0; ii < RANKING_NUM; ii++) {
		//rankEasy[ii] = rankHard[ii] = 0;
		rankEasy[ii] = ii;
	}
}

void Ranking::Draw()
{

}

void Ranking::SetScore(const int& score, const E_LEVEL_ID& level)
{
	// ハイスコアの5番目（最下位）よりスコアが小さかった場合
	if (rankEasy[RANKING_NUM - 1] >= score) return;

	switch (level)
	{
	case E_LEVEL_EASY:
		// ハイスコアの４位から
		for (int ii = RANKING_NUM - 2; ii >= 0; ii--) {
			// ハイスコアより大きかったら
			if (rankEasy[ii] < score)
			{
				// 現在のハイスコアを下位のハイスコアへコピー
				rankEasy[ii + 1] = rankEasy[ii];

				// １位
				if (ii == 0)
				{
					rankEasy[ii] = score;
				}
			}
			else
			{
				// 現在のハイスコアよりスコアが小さかった場合
				rankEasy[ii + 1] = score;
				return;
			}
		}
		break;
	case E_LEVEL_HARD:
		// ハイスコアの４位から
		for (int ii = RANKING_NUM - 2; ii >= 0; ii--) {
			// ハイスコアより大きかったら
			if (rankHard[ii] < score)
			{
				// 現在のハイスコアを下位のハイスコアへコピー
				rankHard[ii + 1] = rankHard[ii];

				// １位
				if (ii == 0)
				{
					rankHard[ii] = score;
				}
			}
			else
			{
				// 現在のハイスコアよりスコアが小さかった場合
				rankHard[ii + 1] = score;
				return;
			}
		}
		break;
	default:
		break;
	}
}