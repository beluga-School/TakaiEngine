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
}

void Mob::Update()
{
	Obj3d::Update(Camera::camera->matView, Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
	rayCol.start = position;
	rayCol.direction = { 0,-1,0 };
}

void Mob::PreJump()
{
	if (onGround)
	{
		jumpPower = 0;
	}
}

void Mob::PostJump()
{
	//�d�͂̉��Z
	if (onGround == false)
	{
		jumpPower += gravity;
	}

	moveValue.y += jumpPower;
}

void Mob::PreMove()
{
	centerVec = matWorld.ExtractAxisZ();
	leftVec = matWorld.ExtractAxisX();
	centerVec.normalize();
	leftVec.normalize();

	moveValue = { 0,0,0 };
}

void Mob::PostMove()
{
	//���W�̍X�V
	position += moveValue;

	//���[���h�s��̍X�V
	Mob::Update();

	//�n�ʂɂ��Ă��邩�̔�������Z�b�g
	onGround = false;
}