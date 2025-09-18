#pragma once

#include <vector>
#include <chrono>
#include "../StDefineData.h"

class SceneBase;


class SceneManager 
{
public:
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	bool SystemInit(void);		// 初期化処理（最初の一回のみ実行）
	void GameInit(void);		// ゲーム起動・再開時に必ず呼び出す処理
	void Update(void);			// 更新処理
	void Draw(void);			// 描画処理
	bool Release(void);			// 解放処理  （最初の一回のみ実行）

	// デルタタイムの取得
	float GetDeltaTime(void) const;
	float GetTotalTime() const { return totalTime_; }

private:
	// 静的インスタンス
	static SceneManager* instance_;

	SceneBase* scene_;

	E_SCENE_ID scene_ID;		// 現在のシーンID

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	LONGLONG NowTime;
	LONGLONG Time;
	float DeltaTime;
	int FPS;
	int FPSCounter;
	LONGLONG FPSCheckTime;

	float totalTime_; // 累積時間（秒）

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	void DeltaTimeInit(void);	// デルタタイムの初期化

	void DeltaTimeUpdate(void);	// デルタタイムの更新

	bool ChangeScene(E_SCENE_ID id);
};