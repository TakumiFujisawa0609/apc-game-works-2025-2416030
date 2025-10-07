#pragma once
#include "ShotBase.h"

class ShotNormal : public ShotBase
{
public:
	// コンストラクタ(元となるモデルのハンドルID)
	ShotNormal(TYPE type, int baseModelId);

	// デストラクタ
	~ShotNormal(void);
private:
	// パラメータ設定
	void SetParam(void) override;
};