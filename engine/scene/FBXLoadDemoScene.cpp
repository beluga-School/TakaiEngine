#include "FBXLoadDemoScene.h"
#include <Sprite.h>
#include <ClearDrawScreen.h>
#include <AssimpLoader.h>
#include "DirectXInit.h"

void FBXLoadDemoScene::Initialize()
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

	//spherefbx.reserve(meshes.size());
	for (size_t i = 0; i < meshes.size(); i++)
	{
		auto vertices = meshes[i].vertices;
		auto indices = meshes[i].indices;

		/*vertexDatas.emplace_back();
		vertexDatas.back().CreateVertex(vertices, indices);*/

		spherefbx.emplace_back();
		spherefbx.back().Initialize();

		/*spherefbx.back().model->CreateVertex(vertices, indices);
		spherefbx.back().model->mesh.vertices = vertices;
		spherefbx.back().model->mesh.indices = indices;*/

		Model *model;
		model = ModelManager::GetModel("Cube");
		model->CreateVertex(vertices, indices);
		model->mMesh.vertices = vertices;
		model->mMesh.indices = indices;

		spherefbx.back().SetModel(model);
	}

	//3dオブジェクト用のパイプライン生成
	object3dPipelineSet = CreateObject3DPipeline();

	camera->Initialize();

	skydome.Initialize();
	skydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	skydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	skydome.scale = { 10,10,10 };
	skydome.color_ = { 1.f,1.f,1.f,1.0f };

	debugCamera.Initialize();
}

void FBXLoadDemoScene::Update()
{
	camera->UpdatematView();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Update(*camera);
	}

	skydome.Update(*camera);

	debugCamera.Update();
}

void FBXLoadDemoScene::Draw()
{
	BasicObjectPreDraw(object3dPipelineSet);

	skydome.DrawMaterial();

	for (size_t i = 0; i < meshes.size(); i++)
	{
		spherefbx[i].Draw();
	}

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::mSpriteCommon);

}

void FBXLoadDemoScene::End()
{
}
