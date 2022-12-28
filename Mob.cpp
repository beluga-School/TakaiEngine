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
	Obj3d::Update(Camera::camera->matView, Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
	rayCol.start = position;
	rayCol.direction = { 0,-1,0 };
}

void Mob::AddGravity()
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
	//移動量を初期化
	moveValue = { 0,0,0 };

	//地面についているかの判定をリセット
	onGround = false;

	centerVec = matWorld.ExtractAxisZ();
	leftVec = matWorld.ExtractAxisX();
	centerVec.normalize();
	leftVec.normalize();
}

void Mob::PostMove()
{
	//座標の更新
	position += moveValue;

	//ワールド行列の更新
	Mob::Update();
}