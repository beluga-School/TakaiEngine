#pragma once
#include "Obj.h"
#include "Collision.h"

class Mob : public Obj3d
{
public:
	Mob();

	float gravity;
	float jumpPower;

	Cube cubeCol;
	Ray rayCol;
};

class Block : public Obj3d
{
public:
	Cube cubeCol;
};
