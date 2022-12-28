#include "Stage.h"
#include "TimeManager.h"
#include "ImguiManager.h"
#include "MathF.h"

void Stage::SetStage1()
{
	SetBlock({ 0,-30,0 }, { 30,50,30 });
	SetBlock({ 30,-20,0 }, { 30,50,30 });
	SetBlock({ -30,-20,0 }, { 30,50,30 });
	SetBlock({ 0,-20,-30 }, { 30,50,30 });
	SetBlock({ 0,-20,30 }, { 30,50,30 });
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

GUI gui3("hoge2");

void Stage::CheckMobCol(Mob& mob)
{
	Cube tempCol = mob.cubeCol;

	//���ړ�
	//�����ňړ��l�𑫂��̂�����A�ړ��l���Ɍ��߂Ă����Ȃ���΂Ȃ�Ȃ�
	tempCol.position += mob.moveValue;

	gui3.Begin({ 800,100 }, { 500,300 });

	for (Block& block : blockList)
	{
		//���̃u���b�N�̏�ʂ���ɂ��邩(���̃u���b�N�̏�ɗ����Ă��邩)
		bool up = tempCol.position.y > block.position.y + block.scale.y * 0.5f;
		//���̃u���b�N�̐��ʂ���O�ɂ��邩
		bool center = tempCol.position.z > block.position.z + block.scale.z * 0.5f;

		bool back = tempCol.position.z < block.position.z - block.scale.z * 0.5f;
		
		bool left = tempCol.position.x > block.position.x + block.scale.x * 0.5f;
		
		bool right = tempCol.position.x < block.position.x - block.scale.x * 0.5f;

		//��ʂ̓����蔻��
		if (up)
		{
			while (CubeCollision(tempCol, block.cubeCol))
			{
				tempCol.position.y += 0.1f;
				mob.moveValue.y += 0.1f;
				mob.jumpPower = 0;
				mob.onGround = true;
			}
		}

		if (up == false && CubeCollision(tempCol, block.cubeCol))
		{
			if (right)
			{
				mob.moveValue.x = 0;
			}
			if (left)
			{
				mob.moveValue.x = 0;
			}
			if (back)
			{
				mob.moveValue.z = 0;
			}
			if (center)
			{
				mob.moveValue.z = 0;
			}
		}
	}

	gui3.End();
}
