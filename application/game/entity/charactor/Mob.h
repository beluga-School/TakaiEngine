#pragma once
#include "Entity.h"
#include "Box.h"

class Mob : public Entity
{
public:
	Vector3 moveValue = { 0,0,0 };

	std::list<Cube> hitListY;

	Mob() {
		SetTag(TagTable::Mob);
	};

	//trueになると重力を受けなくする(gravityの値も0にする)
	void SetNoGravity(bool flag)
	{
		if (!flag)gravity = 0;
		noGravity = flag;
	}
	void SetNoCollsion(bool flag)
	{
		mNoCollision = flag;
	}

	//プレイヤーの足元のオブジェクトの座標
	float GetFeet() {
		return feet;
	};

	//ジャンプの状態管理ステート
	enum class JumpState
	{
		None,	//ジャンプしていない
		Up,		//上昇中
		Staying,//滞空時間
		Down,
	};

	JumpState GetJumpState() const{
		return jumpState;
	};

protected:
	bool noGravity = false;

	//当たり判定を行わないようにする
	bool mNoCollision = false;

	void CollsionUpdate();

	///---縦移動
	JumpState jumpState = JumpState::None;

	//上昇イージングの始点と終点
	float upJumpS = 0;
	float upJumpE = 0;

	//ジャンプ力
	const float jumpPower = 5.0f;

	//上昇管理タイマー
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//滞空時間タイマー
	TEasing::easeTimer stayManageTimer = 0.01f;

	//重力
	float gravity = 0.0f;
	//重力加速度
	const float gravityAdd = 1.5f;

	float hitCubeMaxY = 0;

	float feet = 0;

private:
	//---縦方向の更新
	
	//縦方向の更新まとめ用
	void UpdateY();
	//下方向との押し戻し処理
	void GroundCol();
	//重力とジャンプ機能の更新
	void JumpUpdate();
};
