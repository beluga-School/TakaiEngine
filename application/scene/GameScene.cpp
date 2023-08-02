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

void GameScene::LoadResource()
{
	Stage::Get()->goalSystem.LoadResource();

	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/stage_stageselect", "stage_stageselect");
	LevelLoader::Get()->Load("Scene/stage_castle_outside", "stage_castle_outside");
	LevelLoader::Get()->Load("Scene/stage_castle_inside", "stage_castle_inside");
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands");
	LevelLoader::Get()->Load("Scene/stage_mountain", "stage_mountain");
	LevelLoader::Get()->Load("Scene/stage_graveyard", "stage_graveyard");
	
	//新規シーンを登録して、登録してあるシーンから選んで飛ぶ方式にしたい
	//マップからハンドル名の一覧を取得
	handles = Util::GetKeys(LevelLoader::Get()->GetDataMap());

	UI::LoadResource();
	Player::LoadResource();
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

	ParticleManager::GetInstance()->CreatePool();

	//初期ステージを決定
	//output = "stage_mountain";
	output = "stage_stageselect";
	Stage::Get()->Initialize(*LevelLoader::Get()->GetData(output));
}

GUI sceneChangeGUI("operator");

void GameScene::Update()
{
	//ステータスの更新
	StatusManager::Update();

	CollideManager::Get()->StatusUpdate();

	static bool debugCam = false;

	if (Input::Keyboard::TriggerKey(DIK_R))
	{
		Stage::Get()->Reload();
	}

	mSkydome.Update();

	Stage::Get()->Update();

	sceneChangeGUI.Begin({ 100,100 }, { 300,350 });

	ImGui::Text("fps %f", TimeManager::fps);

	if (ImGui::Button("ShowModel"))
	{
		Stage::Get()->mShowModel = !Stage::Get()->mShowModel;
	}
	if (ImGui::Button("ShowCollider"))
	{
		Stage::Get()->mShowCollider = !Stage::Get()->mShowCollider;
	}
	if (ImGui::Button("debugCam"))
	{
		debugCam = !debugCam;
	}
	if (ImGui::Button("CheckEncountSphere"))
	{
		EnemyManager::Get()->mIsDrawEncountSphere = !EnemyManager::Get()->mIsDrawEncountSphere;
	}
	
	//ハンドルが空でなければ
	if (!handles.empty())
	{
		//ハンドルの一覧をプルダウンで表示
		std::vector<const char*> temp;
		for (size_t i = 0; i < handles.size(); i++)
		{
			temp.push_back(handles[i].c_str());
		}
		static int32_t select = 0;
		ImGui::Combo("StageSelect", &select, &temp[0], (int32_t)handles.size());

		//切り替え用の名前に保存
		output = handles[select];
	}

	if (ImGui::Button("changeScene"))
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->
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
	ImGui::Text("p:HP %d", player->Get()->GetNowHP());

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

	//コリジョンを付けたオブジェクトより前に呼ばれると怖い
	CollideManager::Get()->CollideUpdate();

	pCamera->BackTransparent();

	ParticleManager::GetInstance()->Update();

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

	ParticleManager::GetInstance()->Draw();
	
	//pCamera->Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"));

	SpriteCommonBeginDraw();

	player->DrawUI();

	Stage::Get()->DrawSprite();
}

void GameScene::End()
{
	ParticleManager::GetInstance()->AllDelete();
}
