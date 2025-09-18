#pragma once

//----------------------------
// 列挙型定義
//----------------------------
enum E_SCENE_ID {
	E_SCENE_NON = -1,
	E_SCENE_TITLE,				// タイトル画面
	E_SCENE_GAME,				// ゲームプレイ画面
	E_SCENE_GAMEOVER,			// ゲームオーバー画面
	E_SCENE_GAMECLEAR,			// ゲームクリア画面

	E_SCENE_ID_MAX,
};

// レベル
enum E_LEVEL_ID {
	E_LEVEL_NON = -1,
	E_LEVEL_EASY,				// 簡単
	E_LEVEL_HARD,				// 難しい

	E_LEVEL_MAX,
};