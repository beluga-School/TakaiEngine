#pragma once
#include "Enemy.h"

/*! Bombking
	ボムキング型の敵を配置するクラス(AI未実装)
*/
class Bombking : public Enemy
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

	void Encount()override;

private:

	void Tracking();

	Vector3 pVec{};

	float mSpeed = 10.0f;

	TEasing::easeTimer damageTimer = 1.0f;
	TEasing::easeTimer reboundTimer = 1.0f;
};

