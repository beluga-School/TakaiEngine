#pragma once
#include "Obj.h"

class Enemy : public Obj3d
{
public:
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	bool IsDead() {
		return isDead;
	}
protected:
	bool isDead = false;
};