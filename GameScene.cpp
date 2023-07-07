#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"
#include "Input.h"

void GameScene::LoadResource()
{
	Stage::Get()->goalSystem.LoadResource();

	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/woods", "woods");

	LevelLoader::Get()->Load("Scene/colliderTest", "colliderTest");

	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside");
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside");
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands");

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_castle_outside"));
}

void GameScene::Initialize()
{
	//初期化
	mSkydome.Initialize();
	mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();

	Stage::Get()->goalSystem.Initialize();
}

GUI sceneChangeGUI("operator");

void GameScene::Update()
{
	static bool debugCam = false;

	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->
			GetData(Stage::Get()->GetNowStageHandle()));
	}
	if (Input::Keyboard::TriggerKey(DIK_F))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_castle_inside"));
	}

	mSkydome.Update();

	Stage::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 300,350 });
	if (ImGui::Button("mShowModel"))
	{
		Stage::Get()->mShowModel = !Stage::Get()->mShowModel;
	}
	if (ImGui::Button("mShowCollider"))
	{
		Stage::Get()->mShowCollider = !Stage::Get()->mShowCollider;
	}
	if (ImGui::Button("debugCam"))
	{
		debugCam = !debugCam;
	}

	ImGui::Text("keyboard");
	ImGui::Text("move:w a s d");
	ImGui::Text("cameraMove:mouse");
	ImGui::Text("space:jump");
	ImGui::Text("n:mouseLock Lock/UnLock change");
	std::string mouseLockStates = "nowMouseLock:";
	if (pCamera->mouseLockChange)
	{
		mouseLockStates += "true";
	}
	else
	{
		mouseLockStates += "false";
	}
	ImGui::Text(mouseLockStates.c_str());
	ImGui::Text("ESC:App Close");
	
	ImGui::Text("");
	ImGui::Text("pad");
	ImGui::Text("move:LStick");
	ImGui::Text("cameraMove:RStick");
	ImGui::Text("A:jump");

	sceneChangeGUI.End();

	player->Update();

	//カメラ更新

	if (debugCam)
	{
		mDebugCamera.Update();
	}
	else
	{
		pCamera->Update();
	}

	EnemyManager::Get()->Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	mSkydome.Draw();
	
	//地面用シェーダーを中で呼んでる
	Stage::Get()->Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	player->Draw();

	EnemyManager::Get()->Draw();

	SpriteCommonBeginDraw();

	Stage::Get()->DrawSprite();
}

void GameScene::End()
{
}
