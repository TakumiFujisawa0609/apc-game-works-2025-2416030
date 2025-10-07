#pragma once
class Mouse
{
public:
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static Mouse& GetInstance(void);

	void Init();
	void Updata();
	void Draw();
	void Release();
	
	VECTOR GetMousePos(void) const { return mousePos_; }
	bool IsMousePosArea(int x, int y, int w, int h);
	bool IsMousePosClickArea(int x, int y, int w, int h);
	bool IsMouseDrag(void);
	bool IsMouseDragEnd(void);
private:
	// 静的インスタンス
	static Mouse* instance_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	Mouse(void);

	// コピーコンストラクタも同様
	Mouse(const Mouse& instance) = default;

	// デストラクタも同様
	~Mouse(void) = default;

	VECTOR mousePos_;
	int ClickX, ClickY, Button, LogType;
	int DrawFlag, DrawX, DrawY, DrawColor;
};

