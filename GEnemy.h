#pragma once
#include "Obj.h"
#include "Enemy.h"
#include "Collision.h"

class GEnemy : public Enemy
{
public:
	void Initialize();
	void Update();
	void Draw();

	void HitEffect();
private:
	Obj3d hitSphere;
};