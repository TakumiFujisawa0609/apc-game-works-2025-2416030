#pragma once
#include "SceneBase.h"
class SceneManager;
class Grid;

class TitleScene : public SceneBase
{

public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:
	Grid* grid_;

	// 画像
	int imgTitle_;

	int fontTitle_;
	int fontPress_;

};
