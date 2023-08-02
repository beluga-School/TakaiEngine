#pragma once
#include "IScene.h"
#include "Skydome.h"
#include "DebugCamera.h"
#include "Stage.h"
#include "Player.h"
#include "PlayerCamera.h"
#include "Billboard.h"
#include "SceneChange.h"
#include "UI.h"

class GameScene : public IScene
{
public:
	void LoadResource()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void End()override;

private:
	Skydome mSkydome;
	DebugCamera mDebugCamera;
	
	Player *player = Player::Get();
	PlayerCamera* pCamera = PlayerCamera::Get();

	//ÉVÅ[ÉìêÿÇËë÷Ç¶óp
	std::string output = "";
	std::vector<std::string> handles;
};

