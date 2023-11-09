#pragma once
#include "TEasing.h"
#include "Status.h"
#include "HPGauge.h"
#include "Mob.h"
#include "ImguiManager.h"

/*! Player
	プレイヤークラス
*/
class Player final : public Mob
{
public:
	//ここら辺の状態管理は統一したいし、Apparranceがここにいると色々不都合がありそう
	//出現処理のもろもろ
	enum class PlayerState
	{
		Normal,
		Dash,
		Apparrance,
		Debug,
	}playerState = PlayerState::Normal;

public:
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();
	void DrawCollider();

	void DrawUI();

	void Reset();

	static Player* Get()
	{
		static Player instance;
		return &instance;
	}

	//呼ぶと1回ジャンプする
	void Jump();

	//現在HPを取得
	int32_t GetNowHP();

	/// <summary>
	/// ダメージを受ける処理
	/// </summary>
	void DamageEffect(int32_t damage);

	//土管から出現する動き
	void ApparranceMove(const Vector3& dokanPos, const Vector3& dokanScale);

	bool GetApparanceEnd();

	PlayerState GetState();

private:
	Player() : Mob()
	{
		SetTag(TagTable::Player);
	};
	~Player() {};

	//プレイヤーのモードを変える(0,通常,1,デバッグモード)
	void ChangeMode(const PlayerState& pState);
	
	bool IsMove();

	void MoveUpdate();

	void ColUpdate();
	void RotaUpdate();
	void DamageUpdate();
	void Fly();

	//HPをその値に書きかえる 最大値より大きい場合、最大値を書き換える
	void HPOverFlow(int32_t value);

public:
	//プレイヤーの回転を管理する変数
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	//設置されたフレームなら立つ
	bool mDokanApparrance = false;

	//前移動(上移動)
	Vector3 preMove = { 0,0,0 };

	//敵との当たり判定用スフィア
	Sphere mEncountCol;

private:

	//hpの最大値　ステータスの最大値は外から変えられるようにしたい
	int32_t MAX_HP = 8;

	Status hp = 8;

	Gauge hpGauge = { {
			Util::WIN_WIDTH / 16,
			Util::WIN_HEIGHT - Util::WIN_HEIGHT / 12
		},8 };

	//無敵時間
	TEasing::easeTimer mutekiTimer = 2.0f;
	//ダメージ受けた後の点滅
	//0.1秒ごとに描画を取り消す
	TEasing::easeTimer flashTimer = 0.05f;

	///---横移動
	//移動速度
	float mSpeed = 7.5f;

	//通常の移動速度
	const float NORMAL_SPEED = 7.5f;

	//ダッシュ時の移動速度
	const float DASH_SPEED = 15.0f;

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

	//収集物関係
	//スターの取得数
	Status starCorrectNum = 0;

	TEasing::easeTimer UIDelayTimer = 0.5f;

	TEasing::easeTimer apparranceTimer = 1.0f;

	Vector3 saveDokanPos{};
	Vector3 saveDokanScale{};
};