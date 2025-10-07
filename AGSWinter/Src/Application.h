#pragma once

#include <string>

class SceneManager;

class Application
{
public:
	static constexpr int SCREEN_SIZE_WID = 1920;							// ゲーム画面の横サイズ
	static constexpr int SCREEN_SIZE_WID_HALF = SCREEN_SIZE_WID / 2;		// ゲーム画面の横半分サイズ
	static constexpr int SCREEN_SIZE_HIG = 1080;							// ゲーム画面の縦サイズ
	static constexpr int SCREEN_SIZE_HIG_HALF = SCREEN_SIZE_HIG / 2;		// ゲーム画面の縦半分サイズ

	static constexpr int SCREEN_SIZE_WID2 = 1080;							// ゲーム画面の横サイズ
	static constexpr int SCREEN_SIZE_HIG2 = 768;							// ゲーム画面の縦サイズ

	// データパス関連
	//-------------------------------------------
	static const std::string PATH_DATA;
	static const std::string PATH_IMAGE;
	static const std::string PATH_MODEL;
	//-------------------------------------------

	Application(void);			// コンストラクタ
	~Application(void);			// デストラクタ
	
	bool SystemInit(void);		// 初期化処理（最初の一回のみ実行）
	void Run(void);				// ゲーム起動
	bool Release(void);			// 解放処理  （最初の一回のみ実行）

private:
};