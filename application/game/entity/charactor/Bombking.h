#pragma once
#include "Enemy.h"

/*! Bombking
	ボムキング型の敵を配置するクラス(AI未実装)
*/
class Bombking : public Enemy
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
public:
	Bombking() : Enemy() {
		SetTag(TagTable::Bombking);
	}

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

	void Encount()override;

	bool ThrowBoxHit(const Mob& mob);

	//投げる処理
	void Throw(Mob& mob);

	void DebugGUI();

private:

	void Tracking();

	float mSpeed = 10.0f;

	TEasing::easeTimer damageTimer = 1.0f;
	TEasing::easeTimer reboundTimer = 1.0f;

	Box throwBox;

	Mob* target = nullptr;

	std::vector<Vector3> inters;

	TEasing::easeTimer throwInterval = 1.0f;
	TEasing::easeTimer throwTimer = 1.0f;

	Vector3 pVec{};

	TEasing::easeTimer rotTime = 10.0f;
	Vector3 startVec{};
	Vector3 endVec{};

	std::vector<ActTable> actTables;
};

