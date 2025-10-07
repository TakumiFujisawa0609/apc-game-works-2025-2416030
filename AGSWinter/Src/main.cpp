#include <DxLib.h>
#include "Application.h"


int WINAPI WinMain(
	_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	Application apl;

	//初期化処理
	if (apl.SystemInit() == false)return -1;

	//ゲーム起動
	apl.Run();

	//解放処理
	apl.Release();

	return 0;									// ゲームの終了
}