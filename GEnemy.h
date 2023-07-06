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
		Attacking,	//突進状態
		Staying,	//攻撃後の後隙 状態終わりで当たり判定をとり、NoneかEncountに遷移
	}state = State::None;

	float mSpeed = 2.5f;	//1秒間の移動速度

	//発見時のジャンプ管理タイマー
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//追跡時間管理タイマー(終わったら再度当たり判定をとり、当たってないなら追跡を終了する)
	TEasing::easeTimer attakingTimer = 3.0f;
	Vector3 attackPosS = {0,0,0};
	Vector3 attackPosE = {0,0,0};
	float attackDistance = 2.0f;

	//プレイヤーへのベクトル
	Vector3 pVec{};

	//攻撃の加速度
	float acceleration = 0.1f;

	//加速の最大値
	const float MAX_ACCELERATION = 30.0f;

	//追跡時の横揺れ管理タイマー
	TEasing::easeTimer metronomeTimer = 0.5f;
	//攻撃後の後隙時間
	TEasing::easeTimer stayTimer = 1.0f;

	//ターゲットマークの1回転の時間
	TEasing::easeTimer markRotaTimer = 0.5f;

	//攻撃前の警告マークモデル
	Obj3d targetMark;
};