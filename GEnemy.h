#pragma once
#include "Obj.h"
#include "Enemy.h"
#include "Collision.h"
#include "TEasing.h"
#include "MathF.h"

class GEnemy : public Enemy
{
public:
	void Initialize();
	void Update();
	void Draw();

	void HitEffect();

	void Encount();

	void ColUpdate();

	bool IsDead()
	{
		return attackState == AttackState::Dead;
	}

private:
	Obj3d hitSphere;

	enum class AttackState
	{
		None,		//立ち止まってる状態
		Encount,	//プレイヤーを発見した状態
		Attacking,	//突進状態
		Staying,	//攻撃後の後隙 状態終わりで当たり判定をとり、NoneかEncountに遷移
		Dead,
	}attackState = AttackState::None;

	float mSpeed = 2.5f;	//1秒間の移動速度

	//発見時のジャンプ管理タイマー
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//追跡時間管理タイマー(終わったら再度当たり判定をとり、当たってないなら追跡を終了する)
	Vector3 attackPosS = {0,0,0};
	Vector3 attackPosE = {0,0,0};
	float attackDistance = 2.0f;

	//プレイヤーへのベクトル
	Vector3 pVec{};

	TEasing::easeTimer accelerationTimer = 0.5f;

	//加速の最大値
	const float MAX_ACCELERATION = 6.0f;

	//追跡時の横揺れ管理タイマー
	TEasing::easeTimer metronomeTimer = 0.5f;
	//攻撃後の後隙時間
	TEasing::easeTimer stayTimer = 1.0f;

	//死亡後の吹っ飛ばされ演出で使うやつ
	//吹っ飛ばされイージングの始点と終点
	Vector3 deadEasingS{};
	Vector3 deadEasingE{};

	TEasing::easeTimer deadTimer = 0.5f;

	//吹っ飛ばされた時の方向
	Vector3 deadDirection;

	//死亡後の吹っ飛ばされ中の回転量(1秒間)(ラジアン)
	float deadRoring = MathF::PIf * 10;

	//重力
	float gravity = 0;
	const float gravityAdd = 0.1f;
};