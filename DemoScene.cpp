#include "DemoScene.h"
#include <ClearDrawScreen.h>
#include <Sprite.h>
#include <SceneManager.h>
#include "GameScene.h"
#include "AssimpLoader.h"

const wchar_t* modelFile = L"Resources/sphere_fbx/sphere.fbx";
std::vector<Obj3d> spherefbx;

std::vector<Mesh> meshes;

void DemoScene::Initialize()
{
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

	sphere.Initialize();
	sphere.SetModel(&ModelManager::GetInstance()->sphereM);
	sphere.SetTexture(&TextureManager::GetInstance()->white);
}

void DemoScene::Update()
{
	if (input->TriggerKey(DIK_K))
	{
		Game* game = new Game();
		sceneManager->SetScene(game);
		//sceneManager->ChangeScene("GAMEPLAY");
	}

	camera->UpdatematView();
	sphere.Update(*camera);

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Update(*camera);
	}
}

void DemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	sphere.Draw();

	DirectX12* dx12 = DirectX12::GetInstance();
	TextureManager* texM = TextureManager::GetInstance();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Draw();
	}

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::spriteCommon);

}

void DemoScene::End()
{

}