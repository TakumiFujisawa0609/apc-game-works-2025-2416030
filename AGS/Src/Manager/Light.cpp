#include "SceneManager.h"
#include "Light.h"

Light::Light()
{
}

Light::~Light()
{
}

void Light::UpdateLight(void)
{
	//if (lightPow_ < 0.01f)
	//{
	//	// 徐々に暗く
	//	lightPow_ += 0.00004f * GetDeltaTime();
	//}
	//else
	//{
	//	// 真っ暗
	//	lightPow_ = 0.08f;
	//	camera_->SetFarClip(100.0f);
	//}

	//float pow = lightPow_;

	//{
	//	LightInfo& L = lights_[0];

	//	SetLightPositionHandle(L.handle, pointLightPos_);

	//	// GetColorF(r,g,b) でOK
	//	auto col = GetColorF(pow, pow, pow, 1.0f);

	//	SetLightDifColorHandle(L.handle, col);
	//	SetLightSpcColorHandle(L.handle, col);

	//	if (!L.isActive)
	//	{
	//		SetLightEnableHandle(L.handle, TRUE);
	//		L.isActive = true;
	//	}
	//}

	//{
	//	LightInfo& L = lights_[1];

	//	SetLightPositionHandle(L.handle, pointLightPos_);

	//	//float pow2 = pow * 0.6f;
	//	auto col = GetColorF(pow2, pow2, pow2, 1.0f);

	//	SetLightDifColorHandle(L.handle, col);
	//	SetLightSpcColorHandle(L.handle, col);

	//	if (!L.isActive)
	//	{
	//		SetLightEnableHandle(L.handle, TRUE);
	//		L.isActive = true;
	//	}
	//}

	bool allInactive = true;

	for (auto& L : lights_)
	{
		if (L.lightPow < MIN_LIGHT_POW)
		{
			// 徐々に暗く
			L.lightPow += minusLight * SceneManager::GetInstance().GetDeltaTime();

			allInactive = false;
		}
		else
		{
			// 真っ暗
			L.lightPow = INIT_LIGHT_POW;
			continue;
		}

		float pow = L.lightPow;

		SetLightPositionHandle(L.handle, L.pos);

		auto col = GetColorF(L.lightPow, L.lightPow, L.lightPow, 1.0f);

		SetLightDifColorHandle(L.handle, col);
		SetLightSpcColorHandle(L.handle, col);

		if (!L.isActive)
		{
			SetLightEnableHandle(L.handle, TRUE);
			L.isActive = true;
		}
	}

	// 例：左上にライト情報を表示
	int y = 10;
	for (size_t i = 0; i < LIGHT_LENGTH; ++i)
	{
		auto& L = lights_[i];
		char buf[256];
		//sprintf_s(buf, "L%zu handle=%d pow=%.6f isActive=%d", i, L.handle, L.lightPow, L.isActive ? 1 : 0);
		//DrawString(10, y, buf, GetColor(255, 255, 255));
		y += 16;
	}

	// 二つとも暗いか
	if (allInactive)
	{
		SceneManager::GetInstance().SetCameraNear(100.0f);
	}
}

void Light::DrawLight(void)
{
#pragma region Step1 ポイントライト
	/*if (CheckHitKey(KEY_INPUT_T)) { pointLightPos_.z = 3.0f; }
	if (CheckHitKey(KEY_INPUT_G)) { pointLightPos_.z -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_R)) { pointLightPos_.y += 3.0f; }
	if (CheckHitKey(KEY_INPUT_Y)) { pointLightPos_.y -= 3.0f; }
	if (CheckHitKey(KEY_INPUT_H)) { pointLightPos_.x += 3.0f; }
	if (CheckHitKey(KEY_INPUT_F)) { pointLightPos_.x -= 3.0f; }*/

	/*for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		SetLightPositionHandle(lights_[i].handle, lights_[i].pos);
		SetLightRangeAttenHandle(lights_[i].handle, 400.0f, 0.000001f, lights_[i].lightPow, 0.0000001f);
	}*/
	//SetLightPositionHandle(lights_[0].handle, lights_[0].pos);
	//SetLightRangeAttenHandle(lights_[0].handle, 400.0f, 0.000001f, 0.000001f, 0.0000001f);
	SetLightPosition(lights_[0].pos);
	SetLightRangeAtten(400.0f, 0.000001f, lights_[0].lightPow, 0.0000001f);

	//SetLightPosition(pointLightPos_);
	//SetLightPositionHandle(lights_[0].handle, { pointLightPos_.x,pointLightPos_.y + 200.0f,pointLightPos_.z });

	//SetLightRangeAtten(400.0f, 0.000001f, lights_[1].lightPow, 0.0000001f);
	// 標準ライトのディフューズカラーを青色にする
	//SetLightDifColor(GetColorF(255.0f, 255.0f, 255.0f, 0.0f));

#ifdef _DEBUG
	//DrawFormatString(10, 10, GetColor(255, 255, 255), "FPS : %.1f", 1.0f / deltaTime_);
#endif // DEBUG

	//pauseMenu_->Draw();

	//DrawFormatString(0, 0, GetColor(255, 255, 255), "%f\n%f", lights_[0].lightPow, lights_[1].lightPow);
	//DrawFormatString(0, 80, GetColor(255, 255, 255), "%d", GetEnableLightHandleNum());
}

void Light::CreateLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].isActive = false;

		VECTOR pos = VGet(0.0f, 0.0f, 0.0f);

		// 元の標準ポイントライトと同じパラメータ
		lights_[i].handle = CreatePointLightHandle(
			pos,
			400.0f,            // Range
			0.000001f,         // Atten0
			0.01f,             // Atten1
			0.0000001f         // Atten2
		);

		lights_[i].lightPow = 0.8f;

		// 最初は無効化
		SetLightEnableHandle(lights_[i].handle, TRUE);
	}
}

void Light::DeleteLight(void)
{
	for (int i = 0; i < LIGHT_LENGTH; i++)
	{
		lights_[i].handle = -1;
		lights_[i].isActive = false;
		lights_[i].lightPow = 0.0f;
	}
}

void Light::CreateSetLight()
{
	// 標準ライトを使わない
	SetLightEnable(true);

	// Ambient（環境光）だけ残す
	//SetLightAmbientHandle(0, GetColorF(0.2f, 0.2f, 0.2f));
}

void Light::SetPointLightPos(VECTOR pos)
{
	pointLightPos_ = pos;
}

void Light::IsPointLightPow(VECTOR pos)
{
	int answer = 0;

	auto max = lights_[0].lightPow;

	for (int i = 1; i < LIGHT_LENGTH; i++)
	{
		// max以上だったら
		if (max <= lights_[i].lightPow)
		{
			max = lights_[i].lightPow;
			answer = i;
		}
	}

	// 
	lights_[answer].lightPow = 0.0000001f;
	lights_[answer].pos = pos;
}
