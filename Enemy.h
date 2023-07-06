#pragma once
#include "Obj.h"
#include "Collision.h"

class Enemy : public Obj3d
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;

	bool IsDead() {
		return isDead;
	}

	Sphere sphereCol{};
	Sphere attackSphereCol{};

protected:
	bool isDead = false;
};