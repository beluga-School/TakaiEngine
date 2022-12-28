#pragma once
#include "Obj.h"
#include "Collision.h"

class Block : public Obj3d
{
public:

	Cube cubeCol;

	Vector3 upVec;
	Vector3 centerVec;
	Vector3 leftVec;

	Block() {
		Initialize();
	};
	
	void Initialize();
	void Update();

	void ExtractVec();
};