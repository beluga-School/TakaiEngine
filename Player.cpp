#include "Player.h"
#include "TimeManager.h"
#include "ImguiManager.h"

GUI gui("hoge");

void Player::Initialize()
{
	gravity = -0.1f;
	jumpPower = 0;
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->def;
	scale = { 5,5,5 };
}

void Player::Update()
{
	PreMove();

	Move();

	Jump();

	PostMove();
}

void Player::Move()
{
	float moveVairitu = 10;

	if (input->PushKey(DIK_D)) {
		moveValue.x -= leftVec.x * moveVairitu * TimeManager::deltaTime;
		moveValue.z -= leftVec.z * moveVairitu * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_A)) {
		moveValue.x += leftVec.x * moveVairitu * TimeManager::deltaTime;
		moveValue.z += leftVec.z * moveVairitu * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_W)) {
		moveValue.x -= centerVec.x * moveVairitu * TimeManager::deltaTime;
		moveValue.z -= centerVec.z * moveVairitu * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_S)) {
		moveValue.x += centerVec.x * moveVairitu * TimeManager::deltaTime;
		moveValue.z += centerVec.z * moveVairitu * TimeManager::deltaTime;
	}
}

void Player::Jump()
{
	PreJump();

	if (input->PushKey(DIK_SPACE) && onGround)
	{
		jumpPower = 1;
	}

	PostJump();
}
