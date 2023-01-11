#pragma once
#include "Mob.h"
#include "Stage.h"

class GroundEnemy : public Mob
{
public:
	void Initialize(Vector3 position);
	void Update(const Stage& stage, const Mob& targetMob);

	void Jump();

	bool isDead = false;
};

