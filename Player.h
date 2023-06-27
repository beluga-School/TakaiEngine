#pragma once
#include "Mob.h"

class Player : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();
private:
	float mSpeed = 7.5f;

	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	Vector3 moveValue = {0,0,0};

	bool checkHit = false;

	float gravity = 0.1f;
};

