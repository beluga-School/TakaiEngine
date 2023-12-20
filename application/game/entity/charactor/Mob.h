#pragma once
#include "Entity.h"
#include "Box.h"

/*! Mob
	敵味方問わず、ブロックと当たり判定を持つ生き物が継承するクラス
*/
class Mob : public Entity
{
public:
	//ジャンプの状態管理ステート
	enum class JumpState
	{
		None,	//ジャンプしていない
		Up,		//上昇中
		Staying,//滞空時間
		Down,
	};

public:	
	Mob() : Entity() {
		SetTag(TagTable::Mob);
		//SetLight();
	};

	//trueになると重力を受けなくする(gravityの値も0にする)
	void SetNoGravity(bool flag)
	{
		if (flag)gravity = 0;
		noGravity = flag;
	}
	void SetNoCollsion(bool flag)
	{
		mNoCollision = flag;
	}

	void SetNoMove(bool flag) {
		mNoMove = flag;
	};

	//プレイヤーの足元のオブジェクトの座標
	float GetFeet() {
		return feet;
	};

	JumpState GetJumpState() const{
		return jumpState;
	};

	void SetInitScale(const Vector3& scale_);

	void SetLight();

protected:
	void SetGravity(float value);

	void ResetGravity();

	void CollsionUpdate();

private:
	//---縦方向の更新

	//縦方向の更新まとめ用
	void UpdateY();

	void UpdateX();

	//hitlistの中から、もっとも近い座標をX,Y,Z方向それぞれで算出する関数
	void CalcNearestHitLists();
	//重力とジャンプ機能の更新
	void JumpUpdate();

public:
	Vector3 moveValue = { 0,0,0 };

	std::list<IDdCube> hitListUp;
	std::list<IDdCube> hitListDown;
	std::list<IDdCube> hitListRight;
	std::list<IDdCube> hitListLeft;
	std::list<IDdCube> hitListCenter;
	std::list<IDdCube> hitListBack;

	//ムーブブロックに当たってるなら座標を記録し、そこに移動値加算前にポジション代入する
	Vector3 moveBlockPosition{};

	bool moveBlockHit = false;

	bool checkray = false;

	//リストの中でもっとも近い座標を保存する変数
	float hitFeetMax = 0;
	float hitCeilingMax = 0;
	float hitRightMin = 0;
	float hitLeftMin = 0;
	float hitCenterMin = 0;
	float hitBackMin = 0;

protected:
	bool noGravity = false;

	//当たり判定を行わないようにする
	bool mNoCollision = false;

	//移動を行えないようにする
	bool mNoMove = false;

	///---縦移動
	JumpState jumpState = JumpState::None;

	//上昇イージングの始点と終点
	float upJumpS = 0;
	float upJumpE = 0;

	//ジャンプ力
	float jumpPower = 5.0f;

	//上昇管理タイマー
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//滞空時間タイマー
	TEasing::easeTimer stayManageTimer = 0.01f;

	//重力
	float gravity = 0.0f;
	//重力加速度
	float gravityAdd = 1.5f;

	//床に引っかからないように掛ける補正値
	const float hitOffset = 0.1f;

	float feet = 0;

	Vector3 initScale = { 0,0,0 };

	//丸影を参照するための番号
	int32_t shadowNum = -1;
};