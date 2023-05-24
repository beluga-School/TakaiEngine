#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"
#include "AssimpLoader.h"
#include <TimeManager.h>

const wchar_t* modelFile = L"Resources/sphere_fbx/sphere.fbx";
std::vector<Obj3d> spherefbx;

std::vector<Mesh> meshes;

void DemoScene::Initialize()
{
	//fbx読み込みの形跡
	ImportSettings importSetting =
	{
		modelFile,
		meshes,
		false,
		false,
	};
	AssimpLoader loader;
	
	if (!loader.Load(importSetting))
	{
		assert(0);
	}

	spherefbx.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx.emplace_back();
		spherefbx.back().Initialize();

		auto vertices = meshes[i].vertices;

		auto indices = meshes[i].indices;

		spherefbx.back().model->CreateVertex(vertices, indices);

	}

	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(&TextureManager::Get()->white);
	skydome.scale = { 10,10,10 };
	skydome.color = { 1.f,1.f,1.f,1.0f };

	/*sphere.Initialize();
	sphere.SetModel(&ModelManager::Get()->sphereM);
	sphere.SetTexture(&TextureManager::Get()->white);*/

	LevelData* data = LevelLoader::Get()->Load("test");

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

	float speed = 100.0f;

	if (input->Get()->PushKey(DIK_D))
	{
		camera->eye.x += speed * TimeManager::deltaTime;
		camera->target.x += speed * TimeManager::deltaTime;
	}
	if(input->Get()->PushKey(DIK_A))
	{ 
		camera->eye.x -= speed * TimeManager::deltaTime;
		camera->target.x -= speed * TimeManager::deltaTime;
	}

	if (input->Get()->PushKey(DIK_W))
	{
		camera->eye.y += speed * TimeManager::deltaTime;
	}
	if (input->Get()->PushKey(DIK_S))
	{
		camera->eye.y -= speed * TimeManager::deltaTime;
	}

	if (input->Get()->PushKey(DIK_UP))
	{
		camera->eye.z += speed * TimeManager::deltaTime;
		camera->target.z += speed * TimeManager::deltaTime;
	}
	if (input->Get()->PushKey(DIK_DOWN))
	{
		camera->eye.z -= speed * TimeManager::deltaTime;
		camera->target.z -= speed * TimeManager::deltaTime;
	}
	if (input->Get()->PushKey(DIK_I))
	{
		camera->eye.y += speed * TimeManager::deltaTime;
		camera->target.y += speed * TimeManager::deltaTime;
	}
	if (input->Get()->PushKey(DIK_K))
	{
		camera->eye.y -= speed * TimeManager::deltaTime;
		camera->target.y -= speed * TimeManager::deltaTime;
	}
	camera->UpdatematView();
	//sphere.Update(*camera);

	/*for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Update(*camera);
	}*/

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
	//sphere.Draw();

	for (auto& obj : obj3ds)
	{
		obj.Draw();
	}

	/*for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Draw();
	}*/

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
