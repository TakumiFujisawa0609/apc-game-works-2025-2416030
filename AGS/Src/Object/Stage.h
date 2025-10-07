#pragma once
class Stage
{
public:
	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	int GetModelId() { return modelId_; }
private:
	// ３ＤモデルのハンドルID
	int modelId_;

	int backModelId_;

	int skyImg;
};

