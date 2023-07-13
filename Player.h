#pragma once
#include "Mob.h"
#include "TEasing.h"

//プレイヤーが個別で持つ機能
//入力
//カメラ

//汎化だけど作りが違う機能
//ジャンプ
//移動
//回転
//汎化で作りも同じ機能
//当たり判定

//リストの中に同じオブジェクトがあれば、そのオブジェクトを入れないプッシュバック
template <class T>
void UniqueObjectPushBack(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら止める
		if (*itr == col)
		{
			return;
		}
		//回しきれたら同じ要素がない
	}
	//から入れる
	list.push_back(col);
}

//リストの中に同じオブジェクトがあれば、そのオブジェクトを削除する
template <class T>
void UniqueObjectErase(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//同じ要素が見つかったら
		if (*itr == col)
		{
			//消す
			list.erase(itr);
			return;
		}
	}
}

//ブロックの方向を確認するためのenum
enum class CheckDirection
{
	CD_UP,
	CD_DOWN,
	CD_CENTER,
	CD_BACK,
	CD_LEFT,
	CD_RIGHT,
};

class Player : public Obj3d
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

	//プレイヤーの足元のオブジェクトの座標
	float GetFeet() {
		return feet;
	};

	bool CheckDirections(const Cube& cubeCol, const Cube& blockCol, const CheckDirection& CD);

	//呼ぶと1回ジャンプする
	void Jump();

private:
	Player(){};
	~Player(){};

	void Attack();

	void SideMoveUpdate();
	void JumpUpdate();
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

	//移動値(横移動)
	Vector3 moveValue = {0,0,0};

	///---縦移動
	enum class JumpState
	{
		None,	//ジャンプしていない
		Up,		//上昇中
		Staying,//滞空時間
		Down,
	}jumpState = JumpState::None;

	//上昇イージングの始点と終点
	float upJumpS = 0;
	float upJumpE = 0;

	//ジャンプ力
	const float jumpPower = 10.0f;

	//上昇管理タイマー
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//滞空時間タイマー
	TEasing::easeTimer stayManageTimer = 0.1f;

	//重力
	float gravity = 0.0f;
	//重力加速度
	const float gravityAdd = 1.5f;

	///---当たり判定
	
	//当たった地面の情報を保存するリスト
	std::list<Cube> hitListY;

	//当たってる地面のなかで一番高い座標(今プレイヤーが立っている平面オブジェクトの座標)
	//この取り方だと、斜め床との判定で困りそう
	float hitCubeMaxY = 0;
	
	float feet = 0;

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

