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
#include "Status.h"
#include "InStageStarUI.h"

void GameScene::LoadResource()
{
	StageChanger::Get()->goalSystem.LoadResource();

	//���[�h(�������ق�������)
	LevelLoader::Get()->Load("Scene/stage_stageselect", "stage_stageselect",0);
	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside",-1);
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside", -1);
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands", -1);
	LevelLoader::Get()->Load("Scene/stage_mountain", "stage_mountain", 1);
	LevelLoader::Get()->Load("Scene/stage_graveyard", "stage_graveyard", 2);
	LevelLoader::Get()->Load("Scene/starTest", "starTest", -1);
	
	//�V�K�V�[����o�^���āA�o�^���Ă���V�[������I��Ŕ�ԕ����ɂ�����
	//�}�b�v����n���h�����̈ꗗ���擾
	handles = Util::GetKeys(LevelLoader::Get()->GetDataMap());

	UI::StaticLoadResource();
	player->LoadResource();

	InStageStarUI::Get()->LoadResource();

	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Dirt.png", "Dirt");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\FirldMask.png", "FirldMask");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Scales.png", "Scales");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Grass.png", "Grass");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\groundCubeMask.png", "groundCubeMask");

}

void GameScene::Initialize()
{
	//������
	mSkydome.Initialize();
	mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();

	StageChanger::Get()->goalSystem.Initialize();

	ParticleManager::GetInstance()->CreatePool();

	//�����X�e�[�W������
	output = "stage_stageselect";
	StageChanger::Get()->Initialize(*LevelLoader::Get()->GetData(output));

	
}

GUI sceneChangeGUI("operator");

void GameScene::Update()
{
	//�X�e�[�^�X�̍X�V
	StatusManager::Update();

	CollideManager::Get()->StatusUpdate();

	static bool debugCam = false;

	//�����[�h����ۂ̏�������
	//���͂Ń����[�h
	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		StageChanger::Get()->Reload();
	}
	//�v���C���[�̈ʒu���C�̍��W��艺�ɂȂ����烊���[�h
	if (player->position.y < StageChanger::Get()->seaObject->GetPosY())
	{
		StageChanger::Get()->Reload();
	}

	mSkydome.Update();

	StageChanger::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 300,350 });

	ImGui::Text("fps %f", TimeManager::fps);

	if (ImGui::Button("ShowModel"))
	{
		StageChanger::Get()->mShowModel = !StageChanger::Get()->mShowModel;
	}
	if (ImGui::Button("ShowCollider"))
	{
		StageChanger::Get()->mShowCollider = !StageChanger::Get()->mShowCollider;
	}
	if (ImGui::Button("debugCam"))
	{
		debugCam = !debugCam;
	}
	if (ImGui::Button("CheckEncountSphere"))
	{
		EnemyManager::Get()->mIsDrawEncountSphere = !EnemyManager::Get()->mIsDrawEncountSphere;
	}
	
	//�n���h������łȂ����
	if (!handles.empty())
	{
		//�n���h���̈ꗗ���v���_�E���ŕ\��
		std::vector<const char*> temp;
		for (size_t i = 0; i < handles.size(); i++)
		{
			temp.push_back(handles[i].c_str());
		}
		static int32_t select = 0;
		ImGui::Combo("StageSelect", &select, &temp[0], (int32_t)handles.size());

		//�؂�ւ��p�̖��O�ɕۑ�
		output = handles[select];
	}

	if (ImGui::Button("changeScene"))
	{
		StageChanger::Get()->ChangeLevel(*LevelLoader::Get()->
			GetData(output));
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
	
	player->starUI.ValueSliders();
	
	for (auto& down : player->hitListDown)
	{
		ImGui::Text("down.position x:%f y:%f z:%f", down.position.x, down.position.y, down.position.z);
	}

	//player->starUI.ValueSliders();

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

	//�R���W������t�����I�u�W�F�N�g���O�ɌĂ΂��ƕ|��
	CollideManager::Get()->CollideUpdate();

	pCamera->BackTransparent();

	ParticleManager::GetInstance()->Update();

	InStageStarUI::Get()->Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	mSkydome.Draw();
	
	//�n�ʗp�V�F�[�_�[�𒆂ŌĂ�ł�
	StageChanger::Get()->Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	player->Draw();

	EnemyManager::Get()->Draw();

	ParticleManager::GetInstance()->Draw();

	//pCamera->Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"));

	SpriteCommonBeginDraw();

	//�X�e�[�W�Z���N�g�Ȃ獇�v���\��
	if (LevelLoader::Get()->GetData(
		StageChanger::Get()->GetNowStageHandle())->mStageNum == 0)
	{
		player->DrawUI();
	}
	//�����łȂ��Ȃ�X�e�[�W���̎擾���v�\��
	else
	{
		InStageStarUI::Get()->Draw();
	}

	StageChanger::Get()->DrawSprite();

}

void GameScene::End()
{
	ParticleManager::GetInstance()->AllDelete();
}
