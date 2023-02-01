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
	isDead = false;
}

void Player::Initialize()
{
	gravity = -0.1f;
	jumpPower = 0;

	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->white;
	scale = { 5,5,5 };
	moveMag = 30;

	mutekiTimer = 0;
	
	jump = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\jump.wav");
	dash = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\dash.wav");
	hit = SoundManager::GetInstance()->SoundLoadWave("Resources\\sound\\hit.wav");

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

	if (position.y <= -200)
	{
		Respawn();
		HitEffect();
	}

	if (position.y >= 200)
	{
		spawnPos = { 0,250,-560 };
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
		bool down = block.CheckDirections(tempCol, CheckDirection::CD_DOWN);

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

		if (down)
		{
			while (CubeCollision(tempCol, block.cubeCol))
			{
				tempCol.position.y -= 0.1f;
				moveValue.y -= 0.1f;
				jumpPower = 0;
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

		if (up == false && down == false && isWallGrap == false)
		{
			while (CubeCollision(tempCol, block.cubeCol))
			{
				tempCol.position.y += 0.1f;
				moveValue.y += 0.1f;
				jumpPower = 0;
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

		if (input->PushKey(DIK_W) || input->LStickTilt(UP))
		{
			moveValue.y = 20 * TimeManager::deltaTime;
		}
	}

	Jump();

	max = { 1920 / 2,1080 / 2 };

	GetCursorPos(&point);

	static bool hoge = false;

	if (input->TriggerKey(DIK_N))
	{
		hoge = !hoge;
	}

	if (hoge)
	{
		SetCursorPos(max.x, max.y);
		ShowCursor(false);
	
		horizontalRotation += (point.x - max.x) * mouseSpd;
		verticalRotation += (point.y - max.y) * mouseSpd;

		if (verticalRotation > MathF::PI / 2 - MathF::DegConvRad(1.0f)) verticalRotation = MathF::PI / 2 - MathF::DegConvRad(1.0f);
		if (verticalRotation < -MathF::PI / 2 + MathF::DegConvRad(1.0f)) verticalRotation = -MathF::PI / 2 + MathF::DegConvRad(1.0f);

		//回転を設定
		rotation.x = -verticalRotation;
		rotation.y = horizontalRotation;
	}
	else
	{
		ShowCursor(true);

		rotation.x += input->GetRStickMove().y * TimeManager::deltaTime;
		rotation.y += input->GetRStickMove().x * TimeManager::deltaTime;

		if (input->PushKey(DIK_RIGHT))
		{
			rotation.y += TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_LEFT))
		{
			rotation.y -= TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_UP))
		{
			rotation.x += TimeManager::deltaTime;
		}
		if (input->PushKey(DIK_DOWN))
		{
			rotation.x -= TimeManager::deltaTime;
		}

		if (rotation.x > MathF::PI / 2 - MathF::DegConvRad(1.0f)) rotation.x = MathF::PI / 2 - MathF::DegConvRad(1.0f);
		if (rotation.x < -MathF::PI / 2 + MathF::DegConvRad(1.0f)) rotation.x = -MathF::PI / 2 + MathF::DegConvRad(1.0f);
	}
	if (input->TriggerKey(DIK_R))
	{
		rotation.x = 0;
		//rotation.y = 0;
		verticalRotation = 0;
		//horizontalRotation = 0;
	}

	ImGui::Text("Toggle :[N.key] |cameraMouseMoveMode %d", hoge);

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
		if (input->CheckConnectPad() == false)
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
		else
		{
			if (input->LStickTilt(RIGHT)) {
				moveValue.x -= leftVec.x * moveMag * TimeManager::deltaTime;
				moveValue.z -= leftVec.z * moveMag * TimeManager::deltaTime;
			}
			if (input->LStickTilt(LEFT)) {
				moveValue.x += leftVec.x * moveMag * TimeManager::deltaTime;
				moveValue.z += leftVec.z * moveMag * TimeManager::deltaTime;
			}
			if (input->LStickTilt(UP)) {
				moveValue.x -= centerVec.x * moveMag * TimeManager::deltaTime;
				moveValue.z -= centerVec.z * moveMag * TimeManager::deltaTime;
			}
			if (input->LStickTilt(DOWN)) {
				moveValue.x += centerVec.x * moveMag * TimeManager::deltaTime;
				moveValue.z += centerVec.z * moveMag * TimeManager::deltaTime;
			}
		}

		moveValue.x = Util::Clamp(moveValue.x, -1.0f, 1.0f);
		moveValue.z = Util::Clamp(moveValue.z, -1.0f, 1.0f);
	}

	if ((input->TriggerKey(DIK_LSHIFT) || input->TriggerRT(1)) && 
		dashCool <= 0) 
	{
		isDash = true;
		
		dashTimer = 0;
		dashCool = 2;

		dashVec = Camera::camera->target - Camera::camera->eye;
		dashVec.normalize();
		SoundManager::GetInstance()->SoundPlayWave(dash);
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
	if ((input->TriggerKey(DIK_SPACE) || input->TriggerPadButton(XINPUT_GAMEPAD_A)) && onGround)
	{
		jumpPower = 2;
		if (jumpSoundFlag == false)
		{
			SoundManager::GetInstance()->SoundPlayWave(jump);
		}
		jumpSoundFlag = true;
	}

	if ((input->ReleaseKey(DIK_SPACE) || input->ReleasePadButton(XINPUT_GAMEPAD_A)))
	{
		jumpSoundFlag = false;
	}
}

void Player::HitEffect()
{
	if (mutekiTimer <= 0 && isDash == false)
	{
		mutekiTimer = 0.01f;
		hp -= 1;
		SoundManager::GetInstance()->SoundPlayWave(hit);
	}
}

void Player::Draw()
{
	if (blinkTimer % 5 == 0)
	{
		Obj3d::Draw();
	}
}

void Player::End()
{
	SoundManager::GetInstance()->SoundUnload(&jump);
	SoundManager::GetInstance()->SoundUnload(&dash);
	SoundManager::GetInstance()->SoundUnload(&hit);
}
