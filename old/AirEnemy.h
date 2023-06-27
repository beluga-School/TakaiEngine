#pragma once
#include "Mob.h"
#include "Stage.h"
#include "Bullet.h"

class AirEnemy : public Mob
{
public:
	AirEnemy() {
		Initialize({0,0,0});
	};

	void Initialize(const Vector3& position);
	void Update(const Stage& stage, const Mob& targetMob);
	void Draw();

	std::list<Bullet> bullets;
	float bulletTimer = 2.0f;
	float frame = 0;

	bool isDead = false;
};

