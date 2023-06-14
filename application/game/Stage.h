#pragma once
#include "Block.h"
#include <list>

class Stage
{
public:
	std::list<Block> blockList;

	void SetStage1();
	void SetBlock(const Vector3& pos,const Vector3& scale);
	void Update();
	void Draw();
};