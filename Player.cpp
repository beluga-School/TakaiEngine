#include "Player.h"
#include "ViewProjection.h"
#include "TimeManager.h"
#include "ImguiManager.h"

GUI gui("hoge");

void Player::Update()
{
	Move();

	Jump();

	//�n�ʂɖ��܂��Ă�����n�ʂ̏�܂ō��W���ړ�
	if (onGround)
	{
		moveValue.y += 0.1f;
	}

	position += moveValue;

	Obj3d::Update(Camera::camera->matView, Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
	rayCol.start = position;
	rayCol.direction = { -1,0,0 };

	gui.Begin({ 10,10 }, { 100,100 });
	ImGui::Text("onground %d", onGround);

	gui.End();

	//�n�ʂɂ��Ă��邩�̔�������Z�b�g
	onGround = false;
}

void Player::Draw()
{
	Obj3d::Draw();
}

void Player::Move()
{
	moveValue = { 0,0,0 };

	if (input->PushKey(DIK_D)) {
		moveValue.x += 10.0f * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_A)) {
		moveValue.x -= 10.0f * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_W)) {
		moveValue.z += 10.0f * TimeManager::deltaTime;
	}
	if (input->PushKey(DIK_S)) {
		moveValue.z -= 10.0f * TimeManager::deltaTime;
	}
}

void Player::Jump()
{
	if (input->PushKey(DIK_SPACE))
	{
		jumpPower = 1;
	}
	if (onGround == false)
	{
		jumpPower += gravity;
	}

	if (onGround)
	{
		jumpPower = 0;
		
	}

	moveValue.y += jumpPower;

}
