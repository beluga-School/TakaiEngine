#include "FBXLoadDemoScene.h"
#include <Sprite.h>
#include <ClearDrawScreen.h>
#include <AssimpLoader.h>
#include "DirectXInit.h"
#include <StringUtil.h>

void FBXLoadDemoScene::Initialize()
{
	//mSphereFBX.Load(L"Resources/Cube_two/Cube_two.glb");
	mSphereFBX.SetModel(ModelManager::GetModel("Cube_two"));

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

	mSphereFBX.Update();

	mSkydome.Update(*mCamera);

	mDebugCamera.Update();
}

void FBXLoadDemoScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Object3D"));

	mSkydome.DrawMaterial();

	mSphereFBX.Draw();

	//スプライトの前描画(共通コマンド)
	SpriteCommonBeginDraw();

}

void FBXLoadDemoScene::End()
{
}

void Obj3dFBX::Update()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mSpherefbx[i].Update(*Camera::sCamera);
	}
}

void Obj3dFBX::Draw()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mSpherefbx[i].Draw();
	}
}

void Obj3dFBX::DrawMaterial()
{
	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mSpherefbx[i].DrawMaterial();
	}
}

void Obj3dFBX::SetModel(const Model* model)
{
	mMeshesSize = (int32_t)model->mMeshes.size();

	for (int32_t i = 0; i < mMeshesSize; i++)
	{
		mSpherefbx.emplace_back();
		mSpherefbx.back().Initialize();
		mSpherefbx.back().SetTexture(TextureManager::GetTexture("white"));

		mSpherefbx.back().SetModel(model);

	}
}
