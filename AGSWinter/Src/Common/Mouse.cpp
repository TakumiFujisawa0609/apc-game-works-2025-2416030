#include <DxLib.h>
#include "Mouse.h"

Mouse* Mouse::instance_ = nullptr;

void Mouse::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new Mouse();
	}
	instance_->Init();
}

Mouse& Mouse::GetInstance(void)
{
	return *instance_;
}

Mouse::Mouse()
{
}


void Mouse::Init()
{
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);

	// 描画情報の初期化
	DrawFlag = FALSE;
	DrawX = 0;
	DrawY = 0;
	DrawColor = 0;

	mousePos_ = { 0, 0, 0 };
}

void Mouse::Updata()
{
	// マウスのボタンが押されたり離されたりしたかどうかの情報を取得する
	if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, TRUE) == 0)
	{
		mousePos_.x = (float)ClickX;
		mousePos_.y = (float)ClickY;

		// 左ボタンが押されたり離されたりしていたら描画するかどうかのフラグを立てて、座標も保存する
		if ((Button & MOUSE_INPUT_LEFT) != 0 && IsMousePosArea(15,15,200,200))
		{
			DrawFlag = TRUE;
			DrawX = ClickX;
			DrawY = ClickY;

			// 四角形の色を押された場合は白に、離された場合は紫にする
			DrawColor = LogType == MOUSE_INPUT_LOG_DOWN ? GetColor(255, 255, 255) : GetColor(255, 0, 255);
		}
	}
}

void Mouse::Draw()
{
	// 描画するかどうかのフラグが立っていたら、マウスの左ボタンが押されたり離されたりした座標に小さい四角形を描画する
	if (DrawFlag == TRUE)
	{
		DrawBox((int)mousePos_.x - 8, (int)mousePos_.y - 8, (int)mousePos_.x + 8, (int)mousePos_.y + 8, DrawColor, TRUE);
	}
}

void Mouse::Release()
{
	delete instance_;
}

bool Mouse::IsMousePosArea(int x, int y, int w, int h)
{
	if (mousePos_.x >= x && mousePos_.x <= x + w && mousePos_.y >= y && mousePos_.y <= y + h)
	{
		return true;
	}

	return false;
}

bool Mouse::IsMousePosClickArea(int x, int y, int w, int h)
{
#if 0
	if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, TRUE) == 0)
	// 左クリックしたら
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		// マウスの位置を取得
		GetMousePoint(&ClickX, &ClickY);
		mousePos_ = { (float)ClickX, (float)ClickY, 0.0f };
		if (mousePos_.x >= x && mousePos_.x <= x + w && mousePos_.y >= y && mousePos_.y <= y + h)
		{
			ClickX = ClickY = 0;
			Init();

			return true;
		}
	}
#endif
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
	{
		GetMousePoint(&ClickX, &ClickY);

		mousePos_.x = (float)ClickX;
		mousePos_.y = (float)ClickY;

		if (mousePos_.x >= x && mousePos_.x <= x + w && mousePos_.y >= y && mousePos_.y <= y + h)
		{
			//ClickX = ClickY = 0;
			//Init();

			return true;
		}
	}

	return false;
}

bool Mouse::IsMouseDrag(void)
{
	return true;
}

bool Mouse::IsMouseDragEnd(void)
{
	return false;
}
