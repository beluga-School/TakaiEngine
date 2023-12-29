#pragma once
#include "Obj.h"
#include "Collision.h"
#include "Mob.h"
#include "Status.h"

//行動テーブル この中から必要なものだけ抜き出して使う設計
enum class ActTable
{
	None,		//何もしていない状態
	Encount,	//プレイヤーを発見した状態
	Tracking,
	Attack1,
	Attack2,
	Staying,
	Damage,
	Dead,		//死亡状態
};

/*! Enemy
	敵の共通項を取り出したクラス
*/
class Enemy : public Mob
{
public:
	/// <summary>
	/// 指定したステートがあるか確認する
	/// </summary>
	/// <param name="check">確認したいステート</param>
	/// <returns>あったらtrue ないならfalse</returns>
	bool CheckState(ActTable check);

	/// <summary>
	/// 指定したステートを付ける(重複していた場合は付けない)
	/// </summary>
	/// <param name="check">付けたいステート</param>
	/// <returns>ステートのセットに成功でtrue 重複があったらfalse</returns>
	bool SetState(ActTable check);

	/// <summary>
	/// 指定したステートを削除する
	/// </summary>
	/// <param name="check">削除したいステート</param>
	/// <returns>ステートの削除に成功でtrue ステートが見つからない場合false</returns>
	bool DeleteState(ActTable check);

	/// <summary>
	/// 呼んでいる間、外部から特定のステートで固定する
	/// </summary>
	/// <param name="force">固定したいステート</param>
	void ForceState(ActTable force);
public:
	Enemy() : Mob()
	{
		SetTag(TagTable::Enemy);
		SetTag(TagTable::DitherTransparent);
	}

	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;
	virtual ~Enemy() {};

	//ヒット時のダメージを取得(基本1)
	int32_t GetHitDamage();

	//Encountを管理するスフィアの範囲を描画する
	void EncountSphereDraw(const std::string& oldPipeline);

	//HPの最大値を変える
	//現在HPの削れている分は引き継ぐ
	void SetMaxHP(int32_t SET_MAX_HP);

	//アクティブ状態の取得
	bool GetActive();

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
	void TargetTurn(const Vector3& tVec);

	//エンカウント判定をする球の初期化
	void EncountSphereInitialize();
	//エンカウント判定をする球の更新
	void EncountSphereUpdate();

public:
	Sphere sphereCol{};

	Vector3 saveColCenter = {};

protected:
	//接触時のダメージ
	int32_t hitDamage = 1;

	int32_t maxHP = 1;

	Status mHp = 1;

	std::vector<ActTable> actTables;

private:
	//接触範囲のスフィアを描画する用(判定の管理自体はsphereColが行う)
	Obj3d encountSphere;
};