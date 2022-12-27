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
	///---クラス内で使用する関数群

	//横移動
	void Move();
	//縦移動
	void Jump();

	///---シングルトン
	Input* input = Input::GetInstance();
};