#pragma once
#include "Mob.h"
#include "TEasing.h"

class Player : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	///---横移動
	//移動速度
	float mSpeed = 7.5f;

	//方向ベクトル保存
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//移動値(横移動)
	Vector3 moveValue = {0,0,0};

	//前移動(上移動)
	Vector3 preMove = {0,0,0};

	///---縦移動
	enum class JumpState
	{
		None,	//ジャンプしていない
		Up,		//上昇中
		Staying,//滞空時間
		Down,	//下降中
		End,	//終了を明示的に
	};
	JumpState jumpState = JumpState::None;

	std::list<Cube> hitList;

	//上昇イージングの始点と終点
	float upJumpS = 0;
	float upJumpE = 0;

	//下降イージングの始点と終点
	float downJumpS = 0;
	float downJumpE = 0;

	//ジャンプ力
	const float jumpPower = 10.0f;

	//上昇管理タイマー
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//滞空時間タイマー
	TEasing::easeTimer stayManageTimer = 0.1f;

	//重力
	float gravity = 0.0f;
	//重力加速度
	const float gravityAdd = 0.1f;

	///---当たり判定
	//多分自分が当たり判定を取るオブジェクトを管理するリストがここにあった方がいい気がする
};

