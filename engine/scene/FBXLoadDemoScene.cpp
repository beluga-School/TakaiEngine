#include "FBXLoadDemoScene.h"
#include <Sprite.h>
#include <ClearDrawScreen.h>
#include <AssimpLoader.h>
#include "DirectXInit.h"
#include <StringUtil.h>

void FBXLoadDemoScene::Initialize()
{
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
