#include "GroundEnemy.h"

void GroundEnemy::Initialize()
{
	Mob::Initialize();
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->slime;
	position = { 10,30,0 };
	scale = { 3,3,3 };
}

void GroundEnemy::Update()
{
	PreMove();

	Jump();

	//地面にいるときにまっすぐ前に進む
	if (onGround)
	{
		moveValue.x += centerVec.x;
		moveValue.z += centerVec.z;
	}

	PostMove();
}

void GroundEnemy::Jump()
{
	PreJump();

	//地面についたらジャンプする
	if (onGround)
	{
		jumpPower = 2;
	}

	PostJump();
}
