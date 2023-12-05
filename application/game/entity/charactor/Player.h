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
	
	//なんか複数の状態が両立する仕組みにしたい
	//タグと同じような管理方式に変えるか
	//出現処理のもろもろ
	enum class PlayerState
	{
		Normal,		//通常の状態(移動などの処理を行う)
		Dash,		//ダッシュ状態
		Apparrance,	//土管からの出現状態(Normalと被らない)
		InDokan,	//土管への入り状態(Normalと被らない)
		Debug,		//デバッグ状態
		HipDrop,	//ヒップドロップ
	};

	enum class InputMove
	{
		Push,
		Trigger,
	};
public:
	/// <summary>
	/// 指定したステートがあるか確認する
	/// </summary>
	/// <param name="check">確認したいステート</param>
	/// <returns>あったらtrue ないならfalse</returns>
	bool CheckState(PlayerState check);

	/// <summary>
	/// 指定したステートを付ける(重複していた場合は付けない)
	/// </summary>
	/// <param name="check">付けたいステート</param>
	/// <returns>ステートのセットに成功でtrue 重複があったらfalse</returns>
	bool SetState(PlayerState check);

	/// <summary>
	/// 指定したステートを削除する
	/// </summary>
	/// <param name="check">削除したいステート</param>
	/// <returns>ステートの削除に成功でtrue ステートが見つからない場合false</returns>
	bool DeleteState(PlayerState check);

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

	//プレイヤーのモードを変える(0,通常,1,デバッグモード)
	void ChangeState(const PlayerState& pState);

	bool IsMove();

	bool IsJump();

	bool CanWallKick();

	void DebugGUI();

	//プレイヤーの移動入力があったか
	bool GetInputMove(InputMove input);
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

	//ダッシュ状態へ変更する処理
	void ChangeDash();

	//通常状態の更新処理
	void NormalUpdate();
	
	void DashUpdate();

	void DebugUpdate();

	void ApparanceUpdate();

	void HipDropUpdate();

	//判定用オブジェクト群の更新
	void AdjudicationUpdate();

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

	std::vector<PlayerState> playerStates;

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

	//壁キック変数群
	//この時間だけ反対方向に飛ぶ
	TEasing::easeTimer wallKickTimer = 0.1f;
	//壁キックの進行方向
	Vector3 wallKickVec = {};

	//一度壁キックした壁を保存し、連続でキックできないようにする
	IDdCube *saveKickWall = nullptr;

	bool flyMode = false;

	TEasing::easeTimer rotTime = 0.5f;
	TEasing::easeTimer hipDropTimer = 1.0f;

	float jumpRotaX = 0;
	float jumpRotaY = 0;

	Vector3 endRota;
	Quaternion startQ;
	Quaternion endQ;
	Quaternion culQ;
};