#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"

void GameScene::Initialize()
{
	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/woods", "woods");
	LevelLoader::Get()->Load("Scene/stageSelect", "stageSelect");

	LevelLoader::Get()->Load("Scene/stage_1", "stage_1");
	LevelLoader::Get()->Load("Scene/stage_2", "stage_2");
	LevelLoader::Get()->Load("Scene/stage_3", "stage_3");
	LevelLoader::Get()->Load("Scene/stage_4", "stage_4");
	LevelLoader::Get()->Load("Scene/castle", "castle");

	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("castle"));

	//初期化
	mSkydome.Initialize();
	mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();

	testSphere.Initialize();
	testSphere.SetModel(ModelManager::GetModel("Sphere"));
	testSphere.SetTexture(TextureManager::GetTexture("white"));
}

GUI sceneChangeGUI("change");

void GameScene::Update()
{
	static bool debugCam = false;

	mSkydome.Update();

	Stage::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 100,100 });
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

	testSphere.Update(*Camera::sCamera);
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));

	mSkydome.Draw();
	Stage::Get()->Draw();
	player->Draw();
	//pCamera.Draw();

	EnemyManager::Get()->Draw();

	testSphere.Draw();
}

void GameScene::End()
{
}
