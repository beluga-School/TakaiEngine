#include "Player.h"
#include "TimeManager.h"

void Player::Initialize()
{
	gravity = -0.1f;
	jumpPower = 0;
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->def;
	scale = { 5,5,5 };
	moveMag = 10;
}

void Player::Update()
{
	PreMove();

	Move();

	AddGravity();

	//‰¼ˆÚ“®

	//ƒWƒƒƒ“ƒvˆ—

	//PostMove();
}

void Player::Move()
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
