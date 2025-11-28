#pragma once
#include <chrono>
#include <DxLib.h>
class SceneBase;
class Fader;
class Camera;
class Light;

class SceneManager
{

public:

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		GAME,
		GAMEOVER,
		CLEAR,
	};

	// 重力
	static constexpr float GRAVITY = 9.81f;

	// 配列サイズ
	static constexpr int LIGHT_LENGTH = 2;

	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	// インスタンスの削除
	static void DeleteInstance(void);

	// 初期化
	void Init(void);

	// 3Dの初期設定
	void Init3D(void);

	// 更新
	void Update(void);
	void UpdateLight(void);

	// 描画
	void Draw(void);

	// リソースの破棄
	void Destroy(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId);

	// シーンIDの取得
	SCENE_ID GetSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	Camera& GetCamera(void) const{ return *camera_; }

	void CreateLight(void);
	void DeleteLight(void);

	void CreateSetLight();
	

	void SetPointLightPos(VECTOR pos);

	void IsPointLightPow(VECTOR pos);

	VECTOR GetLightPos() { return pointLightPos_; }

	void SetIsPause(bool isPause) { isPause_ = isPause; }

private:

	struct LightInfo
	{
		bool isActive;   // ライトを使うかどうか
		int handle;      // ライトハンドル
		float lightPow;
		VECTOR pos;
	};

	// 2つ分用意
	LightInfo lights_[LIGHT_LENGTH];

	std::vector<Light> lights;

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneId_;
	SCENE_ID waitSceneId_;

	// フェード
	Fader* fader_;

	// 各種シーン
	SceneBase* scene_;

	// カメラ
	Camera* camera_;

	//PauseMenu* pauseMenu_;
	int rePress, newPress;

	// シーン遷移中判定
	bool isSceneChanging_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	VECTOR pointLightPos_;	//ライトの位置

	float lightPow_;

	// delta time 用 ------
	LONGLONG NowTime;
	LONGLONG Time;
	float DeltaTime;
	int FPS;
	int FPSCounter;
	LONGLONG FPSCheckTime;
	//---------------------

	bool isPause_;
	
	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	SceneManager(void);

	// コピーコンストラクタも同様
	SceneManager(const SceneManager& instance) = default;

	// デストラクタも同様
	~SceneManager(void) = default;

	// デルタタイムをリセットする
	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(SCENE_ID sceneId);

	// フェード
	void Fade(void);
	void DeltaTimeInit(void);
	void DeltaTimeUpdate(void);
};