#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"
#include "Input.h"
#include "MathF.h"

void GameScene::LoadResource()
{
	Stage::Get()->goalSystem.LoadResource();

	//���[�h(�������ق�������)
	LevelLoader::Get()->Load("Scene/woods", "woods");

	LevelLoader::Get()->Load("Scene/colliderTest", "colliderTest");
	LevelLoader::Get()->Load("Scene/test", "test");

	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside");
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside");
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands");

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_grasslands"));
}

void GameScene::Initialize()
{
	//������
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
	sceneChangeGUI.End();

	player->Update();

	//�J�����X�V
	if (debugCam)
	{
		mDebugCamera.Update();
	}
	else
	{
		pCamera->Update();
	}

	EnemyManager::Get()->Update();

	pCamera->BackTransparent();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	mSkydome.Draw();
	
	//�n�ʗp�V�F�[�_�[�𒆂ŌĂ�ł�
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
