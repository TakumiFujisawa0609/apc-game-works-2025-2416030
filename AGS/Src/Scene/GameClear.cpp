#include "GameClear.h"
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"

GameClear::GameClear(void)
{
}

GameClear::~GameClear(void)
{
}

void GameClear::Init(void)
{
	pauseMenu_ = new PauseMenu();

	imgTitle_ = LoadGraph((Application::PATH_IMAGE + "Clear.png").c_str());
}

void GameClear::Update(void)
{
	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE) || ins.IsPadBtnNew(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::DOWN))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}
}

void GameClear::Draw(void)
{
	//DrawString(0,0, "ゲームクリア", GetColor(255, 255, 255));

	DrawRotaGraph(
		Application::SCREEN_SIZE_X / 2,
		Application::SCREEN_SIZE_Y / 2,
		0.5f, 0.0, imgTitle_, true);

	//DrawString((Application::SCREEN_SIZE_X - GetDrawStringWidth("Space/Aで戻る", strlen("Space/Aで戻る"))) / 2, Application::SCREEN_SIZE_Y - 30, "Space/Aで戻る", GetColor(255, 255, 255));
	const char* text = "Space/Aで戻る";

	int w = GetDrawStringWidthToHandle(text, strlen(text), fontPress_);
	int x = (Application::SCREEN_SIZE_X - w) / 2;
	int y = Application::SCREEN_SIZE_Y - 60;

	DrawStringToHandle(x, y, text, GetColor(255, 255, 255), fontPress_);
}

void GameClear::Release(void)
{
	DeleteGraph(imgTitle_);
}
