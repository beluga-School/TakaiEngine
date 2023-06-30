#pragma once
#include "IScene.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "Stage.h"
#include "Player.h"
#include "PlayerCamera.h"

class GameScene : public IScene
{
public:
	void Initialize();
	void Update();
	void Draw();
	void End();

private:
	Skydome mSkydome;
	DebugCamera mDebugCamera;
	
	Player *player = Player::Get();
	PlayerCamera* pCamera = PlayerCamera::Get();
};

