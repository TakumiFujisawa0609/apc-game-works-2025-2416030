#include "GameClear.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "GameOver.h"

GameOver::GameOver(void)
{
}

GameOver::~GameOver(void)
{
}

void GameOver::Init(void)
{
}

void GameOver::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
	{
		Application::GetInstance().GameEnd();
	}
}

void GameOver::Draw(void)
{
	DrawString(0, 0, "ゲームオーバー", GetColor(255, 255, 255));

	DrawString((Application::SCREEN_SIZE_X - GetDrawStringWidth("Space/Aで戻る", strlen("Space/Aで戻る"))) / 2, Application::SCREEN_SIZE_Y - 30, "Space/Aで戻る", GetColor(255, 255, 255));
}

void GameOver::Release(void)
{
}
