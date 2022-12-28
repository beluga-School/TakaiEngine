#include "GroundEnemy.h"

void GroundEnemy::Initialize()
{
	Mob::Initialize();
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->slime;
	position = { 10,30,0 };
	scale = { 3,3,3 };
	moveMag = 5;
}

void GroundEnemy::Update()
{
	PreMove();

	Jump();

	//�n�ʂɂ���Ƃ��ɂ܂������O�ɐi��
	if (onGround)
	{
		moveValue.x += centerVec.x * moveMag;
		moveValue.z += centerVec.z * moveMag;
	}

	PostMove();
}

void GroundEnemy::Jump()
{
	//�n�ʂɂ�����W�����v����
	if (onGround)
	{
		jumpPower = 2;
	}

	AddGravity();
}
