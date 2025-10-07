#pragma once
#include <DxLib.h>

class ShotBase
{
public:
	// 弾の種類
	enum class TYPE
	{
		NORMAL
	};

	// 弾の状態
	enum class STATE
	{
		NONE,
		SHOT,
		BLAST,
		END
	};

	// エフェクトの種類
	enum class EFFECT_TYPE
	{
		GROUND,
		HIT
	};

	// コンストラクタ(弾種別、元となるモデルのハンドルID)
	ShotBase(TYPE type, int baseModelId);

	// デストラクタ
	virtual ~ShotBase(void);

	// 弾の生成(表示開始座標、弾の進行方向)
	virtual void CreateShot(VECTOR pos, VECTOR dir);

	// 更新ステップ
	virtual void Update(void);

	// 描画
	void Draw(void);

	// 解放処理
	void Release(void);

	void ChangeState(STATE state);

	void Blast(EFFECT_TYPE type);

	// 生存判定
	bool IsAlive(void) { return state_ != STATE::END; }
	bool IsCollisionState(void) { return state_ == STATE::SHOT; }

	// 弾の座標
	VECTOR GetPos(void) { return pos_; }

	// 弾の衝突判定用半径
	float GetCollisionRadius(void) { return collisionRadius_; }

	// 弾の種別
	TYPE GetType(void) { return type_; }
protected:
	// 弾の種別
	TYPE type_;
	// 弾の状態
	STATE state_;
	// エフェクトの種類
	EFFECT_TYPE effectType_;


	// 弾のモデルID
	int modelId_;

	// 方向
	VECTOR dir_;
	// 弾の大きさ
	VECTOR scl_;
	// 弾の回転
	VECTOR rot_;
	// 弾の座標
	VECTOR pos_;

	// 弾の移動速度
	float speed_;

	// 弾の生存期間
	int cntAlive_;

	// 弾の衝突判定用半径
	float collisionRadius_;

	// 重力
	float gravityPow_;

	// 爆発エフェクトのリソースハンドル
	//int effectBlastResId_;

	// 爆発エフェクトのプレイハンドル
	int effectBlastPlayId_;

	// パラメータ設定
	virtual void SetParam(void) = 0;

	// 弾の生存期間の減少
	void ReduceCntAlive(void);

	// 状態遷移
	void ChangeShot(void);
	void ChangeBlast(void);
	void ChangeEnd(void);

	// 状態別更新
	void UpdateBlast(void);
	void UpdateShot(void);
	void UpdateEnd(void);

	// 状態別描画
	void DrawBlast(void);
	void DrawShot(void);
	void DrawEnd(void);
};