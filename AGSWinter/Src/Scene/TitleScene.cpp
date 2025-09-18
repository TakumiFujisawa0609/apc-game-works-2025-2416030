//-------------------------------------------------------------------
// ゲーム制作：AGS
// 氏名　　　：2416030　野本　アレン
//-------------------------------------------------------------------
// Application class
#include <DxLib.h>
#include "TitleScene.h"


TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{

}


void TitleScene::Init(void)
{
	// 次のシーンの初期設定
	nextSceneID = E_SCENE_TITLE;
}

void TitleScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		nextSceneID = E_SCENE_GAME;
	}
}

void TitleScene::Draw(void)
{
	DrawString(100, 100, "Title Scene", GetColor(255, 255, 255));
}


void TitleScene::Release(void)
{
}