#include "GroundEnemy.h"
#include "ImguiManager.h"
#include "TimeManager.h"
#include "MathF.h"

void GroundEnemy::Initialize(const Vector3& position_)
{
	Mob::Initialize();
	model = ModelManager::GetModel("beetle");
	texture = TextureManager::GetTexture("slime");
	position = position_;
	scale = { 5,5,5 };
	rotation.y = -MathF::PIf / 2;
	moveMag = 5;
}

void GroundEnemy::Update(const Stage& stage, const Mob& targetMob)
{
	PreMove();

	AddGravity();

	if (position.y <= -150.0f)
	{
		isDead = true;
	}

	Cube tempCol = cubeCol;

	//縦の当たり判定を先に行う
	for (const Block& block : stage.blockList)
	{
		bool up = block.CheckDirections(tempCol, CheckDirection::CD_UP);
		bool center = block.CheckDirections(tempCol, CheckDirection::CD_CENTER);
		bool back = block.CheckDirections(tempCol, CheckDirection::CD_BACK);
		bool left = block.CheckDirections(tempCol, CheckDirection::CD_LEFT);
		bool right = block.CheckDirections(tempCol, CheckDirection::CD_RIGHT);

		//上面の当たり判定
		while (CubeCollision(tempCol, block.cubeCol))
		{
			tempCol.position.y += 0.1f;
			moveValue.y += 0.1f;
			jumpPower = 0;
			onGround = true;
		}
	}

	float distance = 100.0f;

	if (MathF::Abs(position.x - targetMob.position.x) <= distance &&
		MathF::Abs(position.y - targetMob.position.y) <= distance &&
		MathF::Abs(position.z - targetMob.position.z) <= distance)
	{
		moveValue.x += (targetMob.position.x - position.x) * TimeManager::deltaTime;
		moveValue.z += (targetMob.position.z - position.z) * TimeManager::deltaTime;
	}

	//仮移動
	//ここで移動値を足すのだから、移動値を先に決めておかなければならない
	tempCol.position += moveValue;

	for (const Block& block : stage.blockList)
	{
		bool up = block.CheckDirections(tempCol, CheckDirection::CD_UP);
		bool center = block.CheckDirections(tempCol, CheckDirection::CD_CENTER);
		bool back = block.CheckDirections(tempCol, CheckDirection::CD_BACK);
		bool left = block.CheckDirections(tempCol, CheckDirection::CD_LEFT);
		bool right = block.CheckDirections(tempCol, CheckDirection::CD_RIGHT);

		if (up == false && CubeCollision(tempCol, block.cubeCol))
		{
			if (right)
			{
				moveValue.x = 0;
				jumpPower = 3;
			}
			if (left)
			{
				moveValue.x = 0;
				jumpPower = 3;
			}
			if (back)
			{
				moveValue.z = 0;
				jumpPower = 3;
			}
			if (center)
			{
				moveValue.z = 0;
				jumpPower = 3;
			}
		}
	}
	//Jump();

	PostMove();
}

void GroundEnemy::Jump()
{
	//地面についたらジャンプする
	if (onGround)
	{
		jumpPower = 2;
	}
}
