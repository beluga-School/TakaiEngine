#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"

void GameScene::Initialize()
{
	mSkydome.Initialize();
	mDebugCamera.Initialize();

	LevelLoader::Get()->Load("Scene/checkCollider", "checkCollider");
	LevelLoader::Get()->Load("Scene/woods", "woods");

	Stage::Get()->Load(*LevelLoader::Get()->GetData("woods"));

	player.Initialize();
}

void GameScene::Update()
{
	mDebugCamera.Update();
	mSkydome.Update();

	Stage::Get()->Update();

	player.Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Phong"));

	mSkydome.Draw();
	Stage::Get()->Draw();
	player.Draw();
}

void GameScene::End()
{
}
