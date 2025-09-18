#pragma once

#include "../StDefineData.h"

class SceneBase
{

public:

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void) = 0;

	// 初期化処理
	virtual void Init(void) = 0;

	// 更新ステップ
	virtual void Update(void) = 0;

	// 描画処理
	virtual void Draw(void) = 0;

	// 解放処理
	virtual void Release(void) = 0;

	E_SCENE_ID GetNextSceneID(void) const { return nextSceneID; }
protected:
	E_SCENE_ID nextSceneID;		// 次に遷移するシーンのID
};

