#pragma once
#include "Enemy.h"

/*! Bombking
	ボムキング型の敵を配置するクラス(AI未実装)
*/
class Bombking : public Enemy
{
public:
	Bombking() : Enemy() {
		SetTag(TagTable::Bombking);
	}

	void Initialize()override;

	void Update()override;

	void Draw()override;

	//ヒット時に発生すること
	void HitEffect()override;

	//エンカウント時の処理
	void Encount()override;

	//投げボックスとの当たり判定
	bool ThrowBoxHit(const Mob& mob);

	//投げる処理
	void Throw(Mob& mob);

	//Imguiの処理まとめ
	void DebugGUI();

private:
	//追いかける処理まとめ
	void Tracking();

public:

private:

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
};

