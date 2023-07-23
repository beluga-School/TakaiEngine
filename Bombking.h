#pragma once
#include "Enemy.h"

class Bombking : public Enemy
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

	void Encount()override;
};

