#pragma once
#include "Obj.h"
#include "Collision.h"
#include "Mob.h"

class Bullet : public Obj3d
{
public:
	void Initialize(Vector3 position);
	void SetTarget(const Mob &targetMob);
	void Update();
	void Draw();

	Vector3 targetVec;
	Cube cubeCol;
	bool isDead = false;
	float lifeTimer = 3.0f;
};

