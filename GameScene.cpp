#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"

void GameScene::Initialize()
{
	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/checkCollider", "checkCollider");
	LevelLoader::Get()->Load("Scene/woods", "woods");
	LevelLoader::Get()->Load("Scene/stageSelect", "stageSelect");

	LevelLoader::Get()->Load("Scene/stage_1", "stage_1");
	LevelLoader::Get()->Load("Scene/stage_2", "stage_2");
	LevelLoader::Get()->Load("Scene/stage_3", "stage_3");
	LevelLoader::Get()->Load("Scene/stage_4", "stage_4");

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_4"));

	//初期化
	mSkydome.Initialize();
	//mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();
}

GUI sceneChangeGUI("change");

void GameScene::Update()
{
	mSkydome.Update();

	Stage::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 100,100 });
	if (ImGui::Button("woods"))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("woods"));
	}
	if (ImGui::Button("stageSelect"))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stageSelect"));
	}
	sceneChangeGUI.End();

	player->Update();

	//カメラ更新
	//mDebugCamera.Update();
	pCamera->Update();

	EnemyManager::Get()->Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Phong"));

	mSkydome.Draw();
	Stage::Get()->Draw();
	player->Draw();
	//pCamera.Draw();

	EnemyManager::Get()->Draw();
}

void GameScene::End()
{
}
