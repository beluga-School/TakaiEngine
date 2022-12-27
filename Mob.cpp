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
	//重力の加算
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
	//座標の更新
	position += moveValue;

	//ワールド行列の更新
	Mob::Update();

	//地面についているかの判定をリセット
	onGround = false;
}