#pragma once
#include "Obj.h"
#include "Mob.h"
#include "Input.h"

class Player : public Mob
{
public:
	Player() {
		Initialize();
	};

	void Initialize();
	void Update();

private:
	///---�N���X���Ŏg�p����֐��Q

	//���ړ�
	void Move();
	//�c�ړ�
	void Jump();

	///---�V���O���g��
	Input* input = Input::GetInstance();
};