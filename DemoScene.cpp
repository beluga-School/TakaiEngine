#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"
#include "AssimpLoader.h"
#include <TimeManager.h>

void DemoScene::Initialize()
{
	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(&TextureManager::Get()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 1.f,1.f,1.f,1.0f };

	cube.Initialize();
	cube.SetModel(ModelManager::GetModel("Cube"));
	cube.SetTexture(&TextureManager::Get()->white);

	LevelData* data = LevelLoader::Get()->Load("test");

	LevelLoader::Get();

	SetObject(data);

	camera->eye.y = 50;
	camera->eye.z = -150;

	ModelManager::LoadModel("beetle","beetle");
	ModelManager::LoadModel("firewisp","firewisp");
	ModelManager::LoadModel("boss","boss");

	
	slime.SetTexture(TextureManager::GetTexture("slime"));
	slime.position = { 0,0,0 };
}

void DemoScene::Update()
{
	if (input->TriggerKey(DIK_O))
	{
		Game* game = new Game();
		sceneManager->SetScene(game);
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	camera->DebugMove();
	camera->UpdatematView();

	cube.Update(*camera);

	skydome.Update(*camera);
	for (auto& obj : obj3ds)
	{
		obj.Update(*camera);
	}

	slime.Update();
	
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	skydome.DrawMaterial();
	cube.Draw();

	for (auto& obj : obj3ds)
	{
		obj.Draw();
	}

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

	slime.Draw();
}

void DemoScene::End()
{
	obj3ds.clear();
}

void DemoScene::SetObject(LevelData* data)
{
	for (auto& objectData : data->objects)
	{
		//とりあえずキューブで配置
		//TODO:file_nameから逆引きできるようにしたい
		obj3ds.emplace_back();
		obj3ds.back().Initialize();
		//モデル設定
		obj3ds.back().SetModel(ModelManager::GetModel(objectData.fileName));
		obj3ds.back().SetTexture(&TextureManager::Get()->white);
		//座標
		obj3ds.back().position = objectData.translation;
		//回転角
		obj3ds.back().rotation = objectData.rotation;
		//大きさ
		obj3ds.back().scale = objectData.scaling;
	}
}
