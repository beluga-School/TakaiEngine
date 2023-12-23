#pragma once
#include "Enemy.h"

class BombSolider : public Enemy
{
public:
	BombSolider() : Enemy()
	{
		SetTag(TagTable::BombSolider);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
	void Encount()override;

private:
	Vector3 pVec;

	TEasing::easeTimer scalingTimer = 1.0f;
	TEasing::easeTimer explosionTimer = 3.0f;

	//爆発直前のスケール
	Vector3 def = { 1.0f,1.0f,1.0f };
};

