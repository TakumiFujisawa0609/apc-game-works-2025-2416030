#pragma once
#include <DxLib.h>

class Camera
{
public:
	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT, // 定点カメラ
		FREE, // フリーモード
	};


	// カメラの初期位置
	static constexpr VECTOR DEFAULT_POS = { 0.0f, 300.0f, -500.0f };

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES = { 17.0f * DX_PI_F / 180.0f, 0.0f, 0.0f };

	//// カメラの初期位置
	//static constexpr VECTOR DEFAULT_POS = { 360.0f, 320.0f, -360.0f };

	//// カメラの初期角度
	//static constexpr VECTOR DEFAULT_ANGLES = { 26.0f * DX_PI_F / 180.0f, -28.0f * DX_PI_F / 180.0f, 0.0f };

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 3.0f;

	// カメラクリップ：FAR
	static constexpr float CAMERA_FAR = 15000.0f;


	Camera(void);
	~Camera(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画前のカメラ設定
	void SetBeforeDraw(void);

	// デバッグ
	void DrawDebug(void);

	// 解放
	void Release(void);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	const VECTOR& GetAngle(void) const { return angles_; }
	const VECTOR& GetPos(void) const { return pos_; }
private:
	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;

	// カメラモード別の制御処理
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
};

