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
		mModelFile,
		mMeshes,
		false,
		false,
	};
	AssimpLoader loader;

	if (!loader.Load(importSetting))
	{
		assert(0);
	}

	//spherefbx.reserve(meshes.size());
	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		auto vertices = mMeshes[i].vertices;
		auto indices = mMeshes[i].indices;

		/*vertexDatas.emplace_back();
		vertexDatas.back().CreateVertex(vertices, indices);*/

		mSpherefbx.emplace_back();
		mSpherefbx.back().Initialize();

		/*spherefbx.back().model->CreateVertex(vertices, indices);
		spherefbx.back().model->mesh.vertices = vertices;
		spherefbx.back().model->mesh.indices = indices;*/

		Model *model;
		model = ModelManager::GetModel("Cube");
		model->CreateVertex(vertices, indices);
		model->mMesh.vertices = vertices;
		model->mMesh.indices = indices;

		mSpherefbx.back().SetModel(model);
	}

	//3dオブジェクト用のパイプライン生成
	mObject3dPipelineSet = CreateObject3DPipeline();

	mCamera->Initialize();

	mSkydome.Initialize();
	mSkydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	mSkydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	mSkydome.scale = { 10,10,10 };
	mSkydome.color_ = { 1.f,1.f,1.f,1.0f };

	mDebugCamera.Initialize();
}

void FBXLoadDemoScene::Update()
{
	mCamera->UpdatematView();

	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		mSpherefbx[i].Update(*mCamera);
	}

	mSkydome.Update(*mCamera);

	mDebugCamera.Update();
}

void FBXLoadDemoScene::Draw()
{
	BasicObjectPreDraw(mObject3dPipelineSet);

	mSkydome.DrawMaterial();

	for (size_t i = 0; i < mMeshes.size(); i++)
	{
		mSpherefbx[i].Draw();
	}

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw(SpriteCommon::mSpriteCommon);

}

void FBXLoadDemoScene::End()
{
}
