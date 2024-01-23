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

	GUI gui = {"check"};

	Player* player = Player::Get();

	std::vector<Cube> debugCubes;
};

