#pragma once
#include "Obj.h"
#include "Collision.h"
#include "CollideManager.h"

class Enemy : public Mob
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;

	Sphere sphereCol{};
};