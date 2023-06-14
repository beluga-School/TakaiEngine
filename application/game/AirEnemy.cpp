#include "AirEnemy.h"
#include "TimeManager.h"
#include "MathF.h"
#include "Util.h"

void AirEnemy::Initialize(const Vector3& position_)
{
	Mob::Initialize();
	model = ModelManager::GetModel("firewisp");
	texture = TextureManager::GetTexture("firewisp");
	position = position_;
	scale = { 8,8,8 };
	rotation.y = -MathF::PIf / 2;
	moveMag = 5;
}

void AirEnemy::Update(const Stage& stage, const Mob& targetMob)
{
	frame++;
	moveValue = { 0,0,0 };

	//rotation = (targetMob.position - position) * TimeManager::deltaTime;

	PreMove();

	//AddGravity();

	Cube tempCol = cubeCol;

	moveValue.y = MathF::SimpleHarmonicMotion(frame, 1.0f, 120.0f);

	//無限ループが発生したので切除
	
	////縦の当たり判定を先に行う
	//for (Block& block : stage.blockList)
	//{
	//	bool up = block.CheckDirections(tempCol, CheckDirection::CD_UP);
	//	bool center = block.CheckDirections(tempCol, CheckDirection::CD_CENTER);
	//	bool back = block.CheckDirections(tempCol, CheckDirection::CD_BACK);
	//	bool left = block.CheckDirections(tempCol, CheckDirection::CD_LEFT);
	//	bool right = block.CheckDirections(tempCol, CheckDirection::CD_RIGHT);

	//	//上面の当たり判定
	//	while (CubeCollision(tempCol, block.cubeCol))
	//	{
	//		tempCol.position.y += 0.1f;
	//		moveValue.y += 0.1f;
	//		jumpPower = 0;
	//		onGround = true;
	//	}
	//}

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

	PostMove();

	float distance = 150.0f;

	if (MathF::Abs(position.x - targetMob.position.x) <= distance &&
		MathF::Abs(position.y - targetMob.position.y) <= distance &&
		MathF::Abs(position.z - targetMob.position.z) <= distance)
	{
		bulletTimer -= TimeManager::deltaTime;
	}

	if (bulletTimer < 0)
	{
		bullets.emplace_back();
		bullets.back().Initialize(position);
		bullets.back().SetTarget(targetMob);

		bulletTimer = 2.0f;
	}
	
	bullets.remove_if([](Bullet& bullet) {
		return bullet.isDead;
		});
}

void AirEnemy::Draw()
{
	Obj3d::Draw();
	for (Bullet &bullet : bullets)
	{
		bullet.Draw();
	}
}