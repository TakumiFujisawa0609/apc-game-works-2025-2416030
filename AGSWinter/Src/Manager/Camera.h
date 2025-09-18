#pragma once
class Camera
{
public:
	constexpr static VECTOR INIT_CAMERA_POS = { 0.0f, 30.0f, -30.0f };

	// カメラの初期角度
	static constexpr VECTOR DEFAULT_ANGLES = { 0.0f, 0.0f, 0.0f };

	// カメラスピード（移動）
	static constexpr float SPEED_MOVE = 10.0f;

	// カメラスピード(度)
	static constexpr float SPEED_ANGLE_DEG = 1.0f;

	Camera();
	~Camera();

	void Init();
	void Update();
	void SetCameraPos();
private:
	VECTOR pos_;

	// カメラ角度(rad)
	VECTOR angles_;
};

