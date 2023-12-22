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
};

