#pragma once
#include "Obj.h"
#include "Collision.h"

class Mob : public Obj3d
{
public:
	Mob();

	virtual void Update() = 0;
	float gravity;
	float jumpPower;

	Cube cubeCol;
	Ray rayCol;
};