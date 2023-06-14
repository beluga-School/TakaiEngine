#include "Mob.h"
#include "ViewProjection.h"

Mob::Mob()
{
	Initialize();
}

void Mob::Initialize()
{
	gravity = -0.1f;
	jumpPower = 0;
	moveMag = 1;
}

void Mob::Update()
{
	Obj3d::Update(*Camera::mCamera);
	cubeCol.position = position;
	cubeCol.scale = scale;
	rayCol.start = position;
	rayCol.direction = { 0,-1,0 };
}

void Mob::AddGravity()
{
	//�d�͂̉��Z
	if (onGround == false)
	{
		jumpPower += gravity;
	}

	moveValue.y += jumpPower;
}

void Mob::CheckHitColStage(Stage& stage)
{
	Cube tempCol = cubeCol;

	//���ړ�
	//�����ňړ��l�𑫂��̂�����A�ړ��l���Ɍ��߂Ă����Ȃ���΂Ȃ�Ȃ�
	tempCol.position += moveValue;

	for (Block& block : stage.blockList)
	{
		bool up = block.CheckDirections(tempCol, CheckDirection::CD_UP);
		bool center = block.CheckDirections(tempCol, CheckDirection::CD_CENTER);
		bool back = block.CheckDirections(tempCol, CheckDirection::CD_BACK);
		bool left = block.CheckDirections(tempCol, CheckDirection::CD_LEFT);
		bool right = block.CheckDirections(tempCol, CheckDirection::CD_RIGHT);

		//��ʂ̓����蔻��
		if (up)
		{
			while (CubeCollision(tempCol, block.cubeCol))
			{
				tempCol.position.y += 0.1f;
				moveValue.y += 0.1f;
				jumpPower = 0;
				onGround = true;
			}
		}

		if (up == false && CubeCollision(tempCol, block.cubeCol))
		{
			if (right)
			{
				moveValue.x = 0;
			}
			if (left)
			{
				moveValue.x = 0;
			}
			if (back)
			{
				moveValue.z = 0;
			}
			if (center)
			{
				moveValue.z = 0;
			}
		}
	}

}

void Mob::PreMove()
{
	//�ړ��ʂ�������
	moveValue = { 0,0,0 };

	//�n�ʂɂ��Ă��邩�̔�������Z�b�g
	onGround = false;

	centerVec = matWorld.ExtractAxisZ();
	leftVec = matWorld.ExtractAxisX();
	centerVec.normalize();
	leftVec.normalize();
}

void Mob::PostMove()
{
	//���W�̍X�V
	position += moveValue;

	//���[���h�s��̍X�V
	Mob::Update();
}