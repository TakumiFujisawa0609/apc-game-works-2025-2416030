#pragma once

class Camera;

class Player
{
public:
	// コンストラクタ
	Player(void);
	// デストラクタ
	~Player(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);
private:
	Camera* camera_;
};

