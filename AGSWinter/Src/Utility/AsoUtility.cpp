#include "DxLib.h"
#include "AsoUtility.h"

float AsoUtility::RadiusF(const float rad)
{
	return rad * (DX_PI_F / 180.0f);
}

float AsoUtility::Deg2RadF(float deg)
{
    return deg * (DX_PI_F / 180.0f);
}