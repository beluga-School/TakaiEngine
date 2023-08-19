#pragma once
#include "TEasing.h"
#include "Status.h"
#include "HPGauge.h"
#include "Mob.h"
#include "UI.h"
#include "StarUI.h"

class Player : public Mob
{
public:
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();

	void DrawUI();

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
	Sphere mEncountCol;

	//呼ぶと1回ジャンプする
	void Jump();

	//現在HPを取得
	int32_t GetNowHP();

	/// <summary>
	/// ダメージを受ける処理
	/// </summary>
	void DamageEffect(int32_t damage);

	StarUI starUI = {
		{171.62f, 402.235f },0.5f
	};

	//設置されたフレームなら立つ
	bool mDokanApparrance = false;

	//土管から出現する動き
	void ApparranceMove(const Vector3& dokanPos, const Vector3& dokanScale);

private:
	//hpの最大値　ステータスの最大値は外から変えられるようにしたい
	int32_t MAX_HP = 8;

	Status hp = 8;

	Gauge hpGauge = {{
			Util::WIN_WIDTH / 16,
			Util::WIN_HEIGHT - Util::WIN_HEIGHT / 12
		},8};

	Player() : Mob()
	{
		SetTag(TagTable::Player);
	};
	~Player(){};

	void Attack();

	void SideMoveUpdate();
	void ColUpdate();
	void RotaUpdate();

	void DamageUpdate();

	//スター取得UIの動き
	void StarUIUpdate();

	bool IsMove();

	//HPをその値に書きかえる 最大値より大きい場合、最大値を書き換える
	void HPOverFlow(int32_t value);

	//無敵時間
	TEasing::easeTimer mutekiTimer = 2.0f;
	//ダメージ受けた後の点滅
	//0.1秒ごとに描画を取り消す
	TEasing::easeTimer flashTimer = 0.05f;

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

	//収集物関係
	//スターの取得数
	Status starCorrectNum = 0;

	TEasing::easeTimer UIDelayTimer = 0.5f;

	//出現処理のもろもろ
	enum class PlayerState
	{
		Normal,
		Apparrance,
	}playerState = PlayerState::Normal;

	TEasing::easeTimer apparranceTimer = 1.0f;

	Vector3 saveDokanPos{};
	Vector3 saveDokanScale{};
};