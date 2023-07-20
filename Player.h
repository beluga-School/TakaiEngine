#pragma once
#include "TEasing.h"
#include "CollideManager.h"
#include "Status.h"

class Player : public Mob
{
public:
	void Initialize();
	void Update();
	void Draw();

	void Reset();

	static Player* Get()
	{
		static Player instance;
		return &instance;
	}

	//プレイヤーの回転を管理する変数
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	//前移動(上移動)
	Vector3 preMove = { 0,0,0 };

	//敵との当たり判定用スフィア
	Sphere playerCol;

	//呼ぶと1回ジャンプする
	void Jump();

private:
	Status hp = 8;

	Player(){};
	~Player(){};

	void Attack();

	void SideMoveUpdate();
	void ColUpdate();
	void RotaUpdate();

	///---横移動
	//移動速度
	float mSpeed = 7.5f;

	//最大値になるまでの加速時間
	TEasing::easeTimer accelerationTimer = 0.25f;

	//最大値から減るまでの減速時間
	TEasing::easeTimer decelerationTimer = 0.25f;

	//移動終わった時の速度
	float mSaveSpeed = 0.0f;

	//最大速度
	const float MAX_SPEED = 7.5f;

	//ダッシュ時の最大速度
	const float MAX_DASH_SPEED = 15.0f;

	Vector3 oldMoveVec = {0,0,0};

	//方向ベクトル保存
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//敵との当たり判定用スフィアの描画
	Obj3d colDrawer;

	///---攻撃
	enum class AttackState
	{
		None,		//攻撃していない
		Attacking,	//攻撃中
		CoolTime,	//クールタイム
	}attackState = AttackState::None;

	//ローリング時間の管理
	TEasing::easeTimer attackingTimer = 1.0f;
	//ローリングの回転モーションの管理
	TEasing::easeTimer attackingMotionTimer = 0.5f;
	//ローリングのクールタイムの管理
	TEasing::easeTimer attackCoolTimer = 0.75f;

	//ローリングの回転数の管理
	int32_t mRolingNum = 3;

	Vector3 mRolingVec = {0,0,0};

	float mRolingSpeed = 15.0f;

	//---スター取得
	enum class StarState
	{
		None,	//未取得
		Get,	//入手モーション中
	}starState = StarState::None;
};