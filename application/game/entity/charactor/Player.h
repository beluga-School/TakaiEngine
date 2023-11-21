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
		InDokan,
		Debug,
	};
	PlayerState playerState = PlayerState::Normal;
	
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
	
	//プレイヤーのモードを変える(0,通常,1,デバッグモード)
	void ChangeMode(const PlayerState& pState);

	bool IsMove();

	bool IsJump();

	bool CanWallKick();

	void DebugGUI();

private:
	Player() : Mob()
	{
		SetTag(TagTable::Player);
	};
	~Player() {};

	void MoveUpdate();

	void ColUpdate();
	void RotaUpdate();
	void DamageUpdate();
	void Fly();

	//HPをその値に書きかえる 最大値より大きい場合、最大値を書き換える
	void HPOverFlow(int32_t value);

	//同じ壁を2連続で蹴っていないか確認する
	bool CheckContinuanceKick(IDdCube* check);

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

	bool rotCheck = 0;

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

	///---当たり判定用の別オブジェクト群
	//敵との当たり判定用スフィアの描画
	Obj3d colDrawer;

	//進行方向を管理するオブジェクトを別で管理
	Obj3d centerObject;

	//収集物関係
	//スターの取得数
	Status starCorrectNum = 0;

	TEasing::easeTimer UIDelayTimer = 0.5f;

	TEasing::easeTimer apparranceTimer = 1.0f;

	Vector3 saveDokanPos{};
	Vector3 saveDokanScale{};

	//横回転の変数群
	TEasing::easeTimer rotaTimer = 0.25f;
	float rotaStart = 0.0f;
	float targetRota = 0.0f;
	float oldTargetRota = 0.0f;

	//ジャンプ回数
	int32_t jumpCount = 0;

	//ジャンプ時に足される回転
	float jumpPlusRotaX = 0;

	//壁キック変数群
	//この時間だけ反対方向に飛ぶ
	TEasing::easeTimer wallKickTimer = 0.1f;
	//壁キックの進行方向
	Vector3 wallKickVec = {};

	//一度壁キックした壁を保存し、連続でキックできないようにする
	IDdCube *saveKickWall = nullptr;

	bool flyMode = false;

	TEasing::easeTimer rotTime = 0.5f;

	Quaternion endQ;
	Vector3 endRota;
	Quaternion startQ;
	Quaternion culQ;
};