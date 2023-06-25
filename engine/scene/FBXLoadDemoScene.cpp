#include "FBXLoadDemoScene.h"
#include <Sprite.h>
#include <ClearDrawScreen.h>
#include <AssimpLoader.h>
#include "DirectXInit.h"
#include <StringUtil.h>
#include "ImguiManager.h"
#include "SceneManager.h"
#include "DemoScene.h"
#include "MultiRenderScene.h"

void FBXLoadDemoScene::Initialize()
{
	sceneID = "FBXDemo";

	mSphereFBX.SetModel(ModelManager::GetModel("Cube_two"));

	mCamera->Initialize();

	mSkydome.Initialize();
	mSkydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	mSkydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	mSkydome.scale = { 10,10,10 };
	mSkydome.color_ = { 1.f,1.f,1.f,1.0f };

	mDebugCamera.Initialize();
}

GUI gui4("SceneChange");

void FBXLoadDemoScene::Update()
{
	mCamera->UpdatematView();

	mSphereFBX.Update();

	mSkydome.Update(*mCamera);

	mDebugCamera.Update();

	gui4.Begin({ 500,100 }, { 10,10 });
	if (ImGui::Button("DemoScene_Change"))
	{
		mSceneManager->ChangeScene<DemoScene>();
	}
	if (ImGui::Button("MultiRenderScene_Change"))
	{
		mSceneManager->ChangeScene<MultiRenderScene>();
	}

	gui4.End();
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
