#pragma once
#include "Enemy.h"

class BombSolider : public Enemy
{
public:
	BombSolider() : Enemy()
	{

	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
	void Encount()override;
};

