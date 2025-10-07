#pragma once
class AsoUtility
{
public:
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// ‰ñ“]Ž²
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// •ûŒü
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_FL = { -1.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_FR = { 1.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_BL = { -1.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_BR = { 1.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	// ƒ‰ƒWƒAƒ“‚É•ÏŠ·
	static float RadiusF(const float rad);

	static float Deg2RadF(float deg);
};