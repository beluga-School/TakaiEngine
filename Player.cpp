#include "Player.h"
#include "TimeManager.h"
#include "ImguiManager.h"
#include "MathF.h"
#include "ViewProjection.h"
#include "ObjParticle.h"

void Player::Respawn()
{
	position = spawnPos;
	rotation = { 0,0,0 };
	jumpPower = 0;
	hp = 10;
	isDead = false;
}

void Player::Initialize()
{
	gravity = -0.1f;
	jumpPower = 0;

	model = &ModelManager::GetInstance()->playerM;
	texture = &TextureManager::GetInstance()->white;
	scale = { 5,5,5 };
	moveMag = 30;

	mutekiTimer = 0;

	Respawn();
}

void Player::Update(const Stage& stage)
{
	if (mutekiTimer > 0)
	{
		mutekiTimer += TimeManager::deltaTime;
		blinkTimer++;
		if (mutekiTimer > 1.5f)
		{
			mutekiTimer = 0;
			blinkTimer = 0;
		}
	}

	if (hp <= 0)
	{
		isDead = true;
	}

	if (input->TriggerKey(DIK_R))
	{
		Respawn();
	}
	if (position.y <= -200)
	{
		Respawn();
		HitEffect();
	}

	isWallGrap = false;

	PreMove();

	Move();

	//重力の加算
	if (onGround == false && isDash == false)
	{
		jumpPower += gravity;
	}

	moveValue.y += jumpPower;

	//当たり判定と移動値を元に仮移動
	//この処理より下に横移動をいじる処理を増やすとバグるかも
	Cube tempCol = cubeCol;
	Vector3 tempMoveValue = moveValue;

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

		//上面の当たり判定
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

		if (up == false)
		{
			if (CubeCollision(tempCol, block.cubeCol))
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

		if (up == false &&
			(MathF::Abs(tempMoveValue.x) > 0 || 
			MathF::Abs(tempMoveValue.z) > 0))
		{
			if (CubeCollision(tempCol, block.cubeCol))
			{
				isWallGrap = true;
			}
		}
	}

	if (onGround && oldGround == false)
	{
		for (int i = 0; i < 10; i++)
		{
			ParticleManager::GetInstance()->CreateCubeParticle(position, { 2,2,2 }, 20.0f, { 0.3f,0.3f,0.3f,0.5f });
		}
	}

	oldGround = onGround;

	if (isWallGrap)
	{
		jumpPower = 0;
		moveValue.y = 0;

		if (input->PushKey(DIK_W))
		{
			moveValue.y = 20 * TimeManager::deltaTime;
		}
	}

	Jump();

	max = { 1920 / 2,1080 / 2 };

	ShowCursor(false);

	GetCursorPos(&point);

	SetCursorPos(max.x, max.y);

	horizontalRotation += (point.x - max.x) * mouseSpd;
	verticalRotation += (point.y - max.y) * mouseSpd;

	if (verticalRotation > MathF::PI / 2 - MathF::DegConvRad(1.0f)) verticalRotation = MathF::PI / 2 - MathF::DegConvRad(1.0f);
	if (verticalRotation < -MathF::PI / 2 + MathF::DegConvRad(1.0f)) verticalRotation = -MathF::PI / 2 + MathF::DegConvRad(1.0f);

	//回転を設定
	rotation.x = -verticalRotation;
	rotation.y = horizontalRotation;

	//座標の更新
	position += moveValue;

	//ワールド行列の更新
	Mob::Update();

	//isWallGrap = false;
}

void Player::Move()
{
	if (isDash == false)
	{
		if (input->PushKey(DIK_D)) {
			moveValue.x -= leftVec.x * moveMag * TimeManager::deltaTime;
			moveValue.z -= leftVec.z * moveMag * TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_A)) {
			moveValue.x += leftVec.x * moveMag * TimeManager::deltaTime;
			moveValue.z += leftVec.z * moveMag * TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_W)) {
			moveValue.x -= centerVec.x * moveMag * TimeManager::deltaTime;
			moveValue.z -= centerVec.z * moveMag * TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_S)) {
			moveValue.x += centerVec.x * moveMag * TimeManager::deltaTime;
			moveValue.z += centerVec.z * moveMag * TimeManager::deltaTime;
		}
	}

	if (input->TriggerKey(DIK_LSHIFT) && dashCool <= 0) {
		isDash = true;
		
		dashTimer = 0;
		dashCool = 2;

		dashVec = Camera::camera->target - Camera::camera->eye;
		dashVec.normalize();
	}
	if (isDash)
	{
		jumpPower = 0;
		float dashMag = 4;

		moveValue.x += (dashVec.x) * moveMag * dashMag * TimeManager::deltaTime;
		moveValue.y += (dashVec.y) * moveMag * dashMag * TimeManager::deltaTime;
		moveValue.z += (dashVec.z) * moveMag * dashMag * TimeManager::deltaTime;
		
		dashTimer += TimeManager::deltaTime;
		ParticleManager::GetInstance()->CreateCubeParticle(position, { 3,3,3 }, 50.0f, { 1.0f,0,0,1.0f });
		if (dashTimer > 0.5f)
		{
			isDash = false;
		}
	}
	if (dashCool >= 0)dashCool -= TimeManager::deltaTime;
}

void Player::Jump()
{
	if (input->PushKey(DIK_SPACE) && onGround)
	{
		jumpPower = 2;
	}
}

void Player::HitEffect()
{
	if (mutekiTimer <= 0 && isDash == false)
	{
		mutekiTimer = 0.01f;
		hp -= 1;
	}
}

void Player::Draw()
{
	if (blinkTimer % 5 == 0)
	{
		Obj3d::DrawMaterial();
	}
}