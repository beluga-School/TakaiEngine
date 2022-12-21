#pragma once
#include "Obj.h"
#include "Mob.h"
#include "Input.h"

class Player : public Mob
{
public:
	Player() {
		gravity = -0.1f;
		jumpPower = 0;
		model = &ModelManager::GetInstance()->darumaM;
	};

	void Update();
	void Draw();

	Vector3 moveValue;

	//�n�ʂɐݒu���Ă��邩
	bool onGround = false;

private:
	//���ړ�
	void Move();
	//�c�ړ�
	void Jump();

	Input* input = Input::GetInstance();
};