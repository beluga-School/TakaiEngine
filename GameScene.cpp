#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "Stage.h"
#include "ImguiManager.h"
#include "EnemyManager.h"
#include "Input.h"
#include "MathF.h"
#include "TimeManager.h"
#include "CollideManager.h"
#include "ObjParticle.h"

void GameScene::LoadResource()
{
	Stage::Get()->goalSystem.LoadResource();

	//���[�h(�������ق�������)
	LevelLoader::Get()->Load("Scene/woods", "woods");

	LevelLoader::Get()->Load("Scene/colliderTest", "colliderTest");
	LevelLoader::Get()->Load("Scene/buriburi", "buriburi");

	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside");
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside");
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands");
	LevelLoader::Get()->Load("Scene/stage_mountain", "stage_mountain");
}

void GameScene::Initialize()
{
	Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData("stage_mountain"));

	//������
	mSkydome.Initialize();
	mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();

	Stage::Get()->goalSystem.Initialize();

	SceneChange::Get()->Initialize();

	ParticleManager::GetInstance()->CreatePool();
}

GUI sceneChangeGUI("operator");

void GameScene::Update()
{
	static bool debugCam = false;

	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		Stage::Get()->Reload();
	}

	mSkydome.Update();

	Stage::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 300,350 });

	ImGui::Text("fps %f", TimeManager::fps);

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
	
	//�V�K�V�[����o�^���āA�o�^���Ă���V�[������I��Ŕ�ԕ����ɂ�����
	ImGui::InputText("scene_name", output, sizeof(output));
	if (ImGui::CollapsingHeader("hogehoge"))
	{
		//const std::string listBox_item = { "hoge","hogehoge" };
		//ImGui::ListBox("hoge", listBox_item.c_str());
		ImGui::Selectable("hoge");
		ImGui::Selectable("hoge1");
		ImGui::Selectable("hoge2");
		ImGui::EndGroup();
	}

	if (ImGui::Button("changeScene"))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->
			GetData(output));
		savename = "";
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

	ImGui::Text("mouseR %f", PlayerCamera::Get()->GetRadius());

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

	CollideManager::Get()->Update();

	EnemyManager::Get()->Update();

	pCamera->BackTransparent();

	if (Input::Keyboard::TriggerKey(DIK_T))
	{
		SceneChange::Get()->Start();
	}

	SceneChange::Get()->Update();

	ParticleManager::GetInstance()->Update();
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

	ParticleManager::GetInstance()->Draw();
	
	//pCamera->Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"));

	SpriteCommonBeginDraw();

	Stage::Get()->DrawSprite();

	SceneChange::Get()->Draw();
}

void GameScene::End()
{
	ParticleManager::GetInstance()->AllDelete();
}
