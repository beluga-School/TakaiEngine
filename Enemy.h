#pragma once
#include "Obj.h"
#include "Collision.h"
#include "Mob.h"

//行動テーブル この中から必要なものだけ抜き出して使う設計
enum class ActTable
{
	None,		//何もしていない状態
	Encount,	//プレイヤーを発見した状態
	Tracking,
	Staying,
	Dead,		//死亡状態
};

class Enemy : public Mob
{
public:
	Enemy() : Mob()
	{
		taglist.push_back(TagTable::Enemy);
	}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;

	/// <summary>
	/// 現在のテーブルが引数と一致しているならtrueを返す
	/// </summary>
	/// <param name="table">ActTableから確認したいステートを入れる</param>
	/// <returns></returns>
	bool GetNowAct(ActTable table) {
		return mActTable == table;
	};

	Sphere sphereCol{};

	int32_t GetHitDamage();

protected:
	/// <summary>
	/// 指定したターゲットへのベクトルを返す
	/// </summary>
	/// <param name="target">対象のターゲット</param>
	/// <returns></returns>
	Vector3 TargetVector(const Mob& target);

	/// <summary>
	/// 指定したターゲットの方向を向く
	/// 今のプログラムだと絶対に正面を向くので、向きを変えるのに時間かかる感じのプログラムにしたい
	/// </summary>
	/// <param name="target"></param>
	void TargetTurn(const Mob& target,const Vector3& tVec);

	void EncountSphereUpdate();

	//接触時のダメージ
	int32_t hitDamage = 1;

	ActTable mActTable = ActTable::None;
};