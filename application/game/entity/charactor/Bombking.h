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

	void HitEffect()override;

	void Encount()override;

	bool ThrowBoxHit(const Mob& mob);

	//投げる処理
	void Throw(Mob& mob);

private:

	void Tracking();

	Vector3 pVec{};

	float mSpeed = 10.0f;

	TEasing::easeTimer damageTimer = 1.0f;
	TEasing::easeTimer reboundTimer = 1.0f;

	Box throwBox;

	Mob* target = nullptr;

	std::vector<Vector3> inters;

	TEasing::easeTimer throwInterval = 1.0f;
	TEasing::easeTimer throwTimer = 1.0f;
};

