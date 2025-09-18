#pragma once

#include "../StDefineData.h"

class Ranking
{
public:
	constexpr static int RANKING_NUM = 5;

	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Ranking& GetInstance(void);

	void Init();
	void Draw();
	void Release() { delete instance_; }

	void SetScore(const int& score, const E_LEVEL_ID& level);
	int GetEasyScore(const int i) { return rankEasy[i]; }
	int GetHardScore(const int i) { return rankHard[i]; }
	
private:
	static Ranking* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Ranking(void);

	// コピーコンストラクタも同様
	Ranking(const Ranking& instance) = default;

	// デストラクタも同様
	~Ranking(void) = default;

	int rankEasy[RANKING_NUM];
	int rankHard[RANKING_NUM];
};

