#include "Stage.h"

void Stage::SetStage1()
{
	SetBlock({ 0,-30,0 }, { 30,10,30 });
	SetBlock({ 30,-20,0 }, { 30,10,30 });
}

void Stage::SetBlock(Vector3 pos, Vector3 scale)
{
	blockList.emplace_back();
	blockList.back().position = pos;
	blockList.back().scale = scale;
	blockList.back().rotation = {0,0,0};
	//blockList.back().cubeCol.upPlane.normal = { 0,1,0 };
}

void Stage::Update()
{
	for (Block& block : blockList)
	{
		block.Update();
	}
}

void Stage::Draw()
{
	for (Block& block : blockList)
	{
		block.Draw();
	}
}

void Stage::CheckMobCol(Mob& mob)
{
	Cube tempCol = mob.cubeCol;
	tempCol.position += mob.moveValue;

	for (Block& block : blockList)
	{
		if (CubeCollision(tempCol, block.cubeCol))
		{
			mob.onGround = true;
		}
	}
}
