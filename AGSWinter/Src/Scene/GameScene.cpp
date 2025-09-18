#include <DxLib.h>
#include "GameScene.h"
#include "../Manager/Camera.h"
#include "../Object/Grid.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}



void GameScene::Init(void)
{	
	nextSceneID = E_SCENE_GAME;

	camera = new Camera();
	camera->Init();

	grid_ = new Grid();
	grid_->Init();
}

void GameScene::Update(void)
{
	if (CheckHitKey(KEY_INPUT_C))
	{
		nextSceneID = E_SCENE_GAMECLEAR;
	}
	if (CheckHitKey(KEY_INPUT_O))
	{
		nextSceneID = E_SCENE_GAMEOVER;
	}

	camera->Update();
}

void GameScene::Draw(void)
{
	camera->SetCameraPos();
	grid_->Draw();
	DrawSphere3D(VGet(0.0f, 0.0f, 0.0f), 200.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), TRUE);

	DrawString(100, 100, "Game Scene", GetColor(255, 255, 255));
}

void GameScene::Release(void)
{
	delete camera;
	camera = nullptr;

	grid_->Release();
	delete grid_;
	grid_ = nullptr;
}