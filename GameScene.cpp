#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"
#include "Input.h"

//提出を分かりやすくするために、コメントつけてプッシュ

void GameScene::LoadResource()
{
}

void GameScene::Initialize()
{
	Stage::Get()->goalSystem.LoadResource();

	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/woods", "woods");

	LevelLoader::Get()->Load("Scene/stage_1", "stage_1");
	LevelLoader::Get()->Load("Scene/stage_2", "stage_2");
	LevelLoader::Get()->Load("Scene/stage_3", "stage_3");
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands");
	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside");
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside");

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_grasslands"));

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
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));

	mSkydome.Draw();
	Stage::Get()->Draw();
	player->Draw();
	//pCamera.Draw();

	EnemyManager::Get()->Draw();

	SpriteCommonBeginDraw();

	Stage::Get()->DrawSprite();
}

void GameScene::End()
{
}
