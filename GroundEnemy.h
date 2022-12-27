#pragma once
#include "Mob.h"

class GroundEnemy : public Mob
{
public:
	GroundEnemy() {
		Initialize();
	};

	void Initialize();
	void Update();

	void Jump();
};

