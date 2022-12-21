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

	//地面に設置しているか
	bool onGround = false;

private:
	//横移動
	void Move();
	//縦移動
	void Jump();

	Input* input = Input::GetInstance();
};