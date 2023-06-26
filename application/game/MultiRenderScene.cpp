#include "MultiRenderScene.h"
#include "ClearDrawScreen.h"
#include "ImguiManager.h"
#include "DemoScene.h"
#include "FBXLoadDemoScene.h"
#include "SceneManager.h"

void MultiRenderScene::Initialize()
{
	sceneID = "MultiRender";

	mSkydome.Initialize();
	mSkydome.SetModel(ModelManager::Get()->GetModel("skydome"));
	mSkydome.SetTexture(TextureManager::Get()->GetTexture("white"));
	mSkydome.scale = { 10,10,10 };
	mSkydome.color_ = { 1.f,1.f,1.f,1.0f };

	mDebugCamera.Initialize();
}

GUI gui3("ChangeScene");

void MultiRenderScene::Update()
{
	mDebugCamera.Update();

	mSkydome.Update(*Camera::sCamera);

	gui3.Begin({ 500,100 }, { 10,10 });
	if (ImGui::Button("DemoScene_Change"))
	{
		mSceneManager->ChangeScene<DemoScene>();
	}
	if (ImGui::Button("FBXScene_Change"))
	{
		mSceneManager->ChangeScene<FBXLoadDemoScene>();
	}

	gui3.End();
}

void MultiRenderScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine(pipeline));

	mSkydome.DrawMaterial();
}

void MultiRenderScene::End()
{
}
