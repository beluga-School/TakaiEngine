#pragma once
#include "Obj.h"
#include "Collision.h"

class Block : public Obj3d
{
public:
	Cube cubeCol;

	Block() {
		Initialize();
	};
	
	void Initialize();
	void Update();
};