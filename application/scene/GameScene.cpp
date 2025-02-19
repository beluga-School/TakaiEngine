#include "GameScene.h"
#include "ClearDrawScreen.h"
#include "LevelLoader.h"
#include "StageChanger.h"
#include "ImguiManager.h"
#include "EnemyManager.h"
#include "Input.h"
#include "MathF.h"
#include "TimeManager.h"
#include "CollideManager.h"
#include "ObjParticle.h"
#include "Status.h"
#include "EventManager.h"
#include "EventCamera.h"
#include "GameUIManager.h"
#include <EventCameraManager.h>
#include "Bombking.h"
#include "InstantDrawer.h"

void GameScene::LoadResource()
{
	StageChanger::LoadResource();

	//ロード(分けたほうがいい)
	LevelLoader::Get()->Load("Scene/stage_stageselect", "stage_stageselect",0);
	LevelLoader::Get()->Load("Scene/stage_grasslands", "stage_grasslands", 2);
	LevelLoader::Get()->Load("Scene/stage_mountain", "stage_mountain", 1);
	LevelLoader::Get()->Load("Scene/stage_graveyard", "stage_graveyard", -1);
	LevelLoader::Get()->Load("Scene/clear1", "clear1", -1);
	LevelLoader::Get()->Load("Scene/stage_desert", "stage_desert", -1);
	LevelLoader::Get()->Load("Scene/stage_demo", "stage_demo", -1);
	LevelLoader::Get()->Load("Scene/stage_freezeland", "stage_freezeland", 0);
	
	//新規シーンを登録して、登録してあるシーンから選んで飛ぶ方式にしたい
	//マップからハンドル名の一覧を取得
	handles = Util::GetKeys(LevelLoader::Get()->GetDataMap());

	player->LoadResource();

	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Dirt.png", "Dirt");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\FirldMask.png", "FirldMask");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Scales.png", "Scales");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Grass.png", "Grass");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\groundCubeMask.png", "groundCubeMask");
	TextureManager::Load("Resources\\desertTex.png", "desertTex");
	TextureManager::Load("Resources\\rockTex.png", "rockTex");

	GameUIManager::LoadResource();

	EventManager::LoadResource();
}

void GameScene::Initialize()
{	
	//初期化
	mSkydome.Initialize();
	mDebugCamera.Initialize();
	pCamera->Initialize();

	player->Initialize();
	EnemyManager::Get()->Initialize();

	StageChanger::Get()->goalSystem.Initialize();

	ParticleManager::Get()->CreatePool();

	//初期ステージを決定
	output = "stage_stageselect";

	EventManager::Get()->Initialize();

	GameUIManager::Get()->Initialize();

	StageChanger::Get()->Initialize(*LevelLoader::Get()->GetData(output));

	if (SceneChange::Get()->IsBlackOut())
	{
		SceneChange::Get()->Open();
	}
}

GUI sceneChangeGUI("operator");

void GameScene::Update()
{
	//ステータスの更新
	StatusManager::Update();

	CollideManager::Get()->StatusUpdate();

	//リロードする際の条件たち
	//プレイヤーの位置が海の座標より下になったらリロード
	if (player->position.y < StageChanger::Get()->seaObject->GetPosY())
	{
		StageChanger::Get()->Reload();
	}

	if (player->CheckState(Player::PlayerState::Debug))
	{
		GameSceneDebugGUI();
	}

	if (Input::Keyboard::TriggerKey(DIK_5))
	{
		//EventManager::Get()->Start("");
	}

	mSkydome.Update();

	StageChanger::Get()->Update();

	player->Update();

	//カメラ更新
	//イベント中ならカメラ変更
	if (EventManager::Get()->GetLineEvent() != nullptr &&
		EventManager::Get()->GetLineEvent()->get()->isUseCamera)
	{
		EventCameraManager::Get()->Update();
	}
	else if(debugCam)
	{
		mDebugCamera.Update();
	}
	//それ以外はプレイヤーに追従
	else
	{
		pCamera->Update();
	}

	LightGroup::Get()->Update();

	EnemyManager::Get()->Update();

	//コリジョンを付けたオブジェクトより前に呼ばれると怖い
	CollideManager::Get()->CollideUpdate();

	EventManager::Get()->Update();

	ParticleManager::Get()->Update();

	//UI更新
	GameUIManager::Get()->Update();
}

void GameScene::Draw()
{
	BasicObjectPreDraw("Skydome");
	mSkydome.Draw();
	
	//地面用シェーダーを中で呼んでる
	StageChanger::Get()->Draw();

	EnemyManager::Get()->Draw();

	ParticleManager::Get()->Draw();

	EventManager::Get()->Draw();

	//pCamera->Draw();

	SpriteCommonBeginDraw();

	//UI描画
	player->DrawUI();

	GameUIManager::Get()->Draw();

	StageChanger::Get()->DrawSprite();
}

void GameScene::End()
{
	ParticleManager::Get()->AllDelete();
}

void GameScene::GameSceneDebugGUI()
{
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
	player->DebugGUI();
	player->CheckGUI();

	ImGui::Text("RunEvents");
	for (auto& run : EventManager::Get()->GetNowEvents())
	{
		ImGui::Text("%s", run->get()->eventName.c_str());
	}
	ImGui::Text("RunEventsEnd");

	PlayerCamera::Get()->CheckDebug();

	EnemyManager* point = EnemyManager::Get();
	for (auto& enemy : point->enemyList)
	{
		Bombking* king = static_cast<Bombking*>(enemy.get());
		if (king != nullptr && king->CheckTag(TagTable::Bombking)) {
			king->DebugGUI();
		}
	}

	sceneChangeGUI.End();
}
