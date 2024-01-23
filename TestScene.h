#pragma once
#include "IScene.h"
#include "Collision.h"
#include "ImguiManager.h"
#include "Player.h"

const int32_t NUM = 300;

class TestScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;
private:
	Obj3d tri3d;

	//std::vector<Obj3d> debugObjects;
	bool isUse[NUM] = {false};
	GUI gui = {"check"};

	Player* player = Player::Get();
};

