#pragma once
#include "TEasing.h"
#include "Status.h"
#include "HPGauge.h"
#include "Mob.h"
#include "ImguiManager.h"
#include "CameraHitCheck.h"
#include "PlayerState.h"

/*! Player
	プレイヤークラス
*/
class Player final : public Mob
{
public:
	//なんか複数の状態が両立する仕組みにしたい
	//タグと同じような管理方式に変えるか
	//出現処理のもろもろ
	//enum class PlayerState
	//{
	//	Normal,		//通常の状態(移動などの処理を行う)
	//	Dash,		//ダッシュ状態
	//	Apparrance,	//土管からの出現状態(Normalと被らない)
	//	InDokan,	//土管への入り状態(Normalと被らない)
	//	Debug,		//デバッグ状態
	//	HipDrop,	//ヒップドロップ
	//};

	enum class InputMove
	{
		Push,
		Trigger,
	};
public:
	/*/// <summary>
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
	bool DeleteState(PlayerState check);*/
	PlayerState* state;
	void ChangeState(PlayerState* newstate);
	std::string showState();

public:
	//リソース読み込み
	void LoadResource();

	void Initialize();
	void Update();
	void Draw();
	//当たり判定描画
	void DrawCollider();

	//UI描画
	void DrawUI();

	//色々初期化(主に当たり判定)
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

	//出現する動きの終了を取得
	bool GetApparanceEnd();

	//プレイヤーのモードを変える(0,通常,1,デバッグモード)
	//void ChangeState(const PlayerState& pState);

	//動いているか
	bool IsMove();

	//ジャンプしているか
	bool IsJump();

	//Imguiの処理
	void DebugGUI();

	//プレイヤーの移動入力があったか
	bool GetInputMove(InputMove input);

	//地面とぶつかった時のパーティクル
	void GroundHitParticle();

	void MoveValuePlus(const Vector3& plus);

	Vector3 GetCenterVec();

	float GetSpeed();

private:
	Player() : Mob()
	{
		SetTag(TagTable::Player);
		state = new PlayerNormal;
	};
	~Player() {
		delete state;
	};

	//動きの更新
	void MoveUpdate();

	//当たり判定の更新
	void ColUpdate();
	
	//回転挙動の更新
	void RotaUpdate();
	
	//ダメージ処理の更新
	void DamageUpdate();
	
	//跳ぶ処理の更新
	void Fly();

	//HPをその値に書きかえる 最大値より大きい場合、最大値を書き換える
	void HPOverFlow(int32_t value);

	//ダッシュ状態へ変更する処理
	void ChangeDash();

	////通常状態の更新処理
	//void NormalUpdate();
	//
	////ダッシュ状態の更新処理
	//void DashUpdate();

	////デバッグモードの更新処理
	//void DebugUpdate();

	////出現処理の更新処理
	//void ApparanceUpdate();

	////ヒップドロップの更新処理
	//void HipDropUpdate();

	//判定用オブジェクト群の更新
	void AdjudicationUpdate();

	//煙のパーティクルをドーナツ型で出現させる(より汎用的な場所に移動したい)
	void DonuteSmoke(Vector3 center);

public:
	//プレイヤーの回転を管理する変数
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	//設置されたフレームなら立つ
	bool mDokanApparrance = false;

	//前移動(上移動)
	Vector3 mPreMove = { 0,0,0 };

	//敵との当たり判定用スフィア
	Sphere mEncountCol;

	//std::vector<PlayerState> mPlayerStates;

	void CheckGUI();

private:
	//hpの最大値　ステータスの最大値は外から変えられるようにしたい
	int32_t MAX_HP = 8;

	Status mHp = 8;

	Gauge mHpGauge = { {
			Util::WIN_WIDTH / 16,
			Util::WIN_HEIGHT - Util::WIN_HEIGHT / 12
		},8 };

	//無敵時間
	TEasing::easeTimer mMutekiTimer = 2.0f;
	//ダメージ受けた後の点滅
	//0.1秒ごとに描画を取り消す
	TEasing::easeTimer mFlashTimer = 0.05f;

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

	const float JUMP_VALUE = 2.5f;

	//方向ベクトル保存
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	///---当たり判定用の別オブジェクト群
	//敵との当たり判定用スフィアの描画
	Obj3d mColDrawer;

	//進行方向を管理するオブジェクトを別で管理
	Obj3d mCenterObject;

	TEasing::easeTimer mApparranceTimer = 1.0f;

	Vector3 mSaveDokanPos{};
	Vector3 mSaveDokanScale{};

	//ジャンプ回数
	int32_t mJumpCount = 0;

	bool mFlyMode = false;

	TEasing::easeTimer mRotTime = 0.5f;
	TEasing::easeTimer mHipDropTimer = 1.0f;

	const int ROTA_VALUE = 18;

	float mJumpRotaX = 0;
	float mJumpRotaY = 0;

	Vector3 mEndRota;
	Quaternion mStartQ;
	Quaternion mEndQ;
	Quaternion mCulQ;

	Vector3 oldMove = { 0,0,0 };
	Vector2 oldPad = { 0,0 };
};