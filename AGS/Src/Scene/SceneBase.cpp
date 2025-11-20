#include "../Manager/InputManager.h"
#include "PauseMenu.h"
#include "SceneBase.h"

SceneBase::SceneBase(void)
{
}

SceneBase::~SceneBase(void)
{
}

void SceneBase::Init(void)
{
}

void SceneBase::Update(void)
{
}

void SceneBase::Draw(void)
{
}

void SceneBase::Release(void)
{
}

void SceneBase::UpdateBase(void)
{
	auto& ins = InputManager::GetInstance();

	// 接続されているゲームパッド１の情報を取得
	InputManager::JOYPAD_IN_STATE padState =
		ins.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);

	rePress = newPress;
	newPress = ins.IsNew(KEY_INPUT_ESCAPE);

	// Escapeキーでポーズ切り替え
	if ((rePress == 0 && newPress == 1) || padState.IsTrgDown)
	{
		pauseMenu_->Toggle();
	}

	if (pauseMenu_->IsActive())
	{
		pauseMenu_->Update();
		return; // ポーズ中はゲーム更新停止
	}

	Update();
}
