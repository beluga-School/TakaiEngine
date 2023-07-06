#pragma once
#include "Obj.h"
#include "Enemy.h"
#include "Collision.h"
#include "TEasing.h"

class GEnemy : public Enemy
{
public:
	void Initialize();
	void Update();
	void Draw();

	void HitEffect();

	void Encount();

	void ColUpdate();

private:
	Obj3d hitSphere;

	enum class State
	{
		None,		//立ち止まってる状態
		Encount,	//プレイヤーを発見した状態
		Traking,	//追いかけてくる状態
		Attacking,	//攻撃中の状態(この状態の終わりに攻撃判定がでる)
		Staying,	//攻撃後の後隙 状態終わりで当たり判定をとり、NoneかEncountに遷移
	}state = State::None;

	float mSpeed = 2.5f;	//1秒間の移動速度

	//発見時のジャンプ管理タイマー
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//追跡時間管理タイマー(終わったら再度当たり判定をとり、当たってないなら追跡を終了する)
	TEasing::easeTimer trackingTimer = 2.0f;

	//追跡時の横揺れ管理タイマー
	TEasing::easeTimer metronomeTimer = 0.5f;
	//攻撃までの時間
	TEasing::easeTimer attackTimer = 1.0f;
	//攻撃後の後隙時間
	TEasing::easeTimer stayTimer = 1.0f;
};