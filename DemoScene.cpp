#include "DemoScene.h"
#include "Camera.h"
#include "ClearDrawScreen.h"

void DemoScene::LoadResource()
{

}

void DemoScene::Initialize()
{
	demoline.Initialize();
	demoline.SetTexture(TextureManager::GetTexture("white"));

	mDebugCamera.Initialize();
}

void DemoScene::Update()
{
	mDebugCamera.Update();
	demoline.Update(*Camera::sCamera);
}

void DemoScene::Draw()
{
	BasicObjectPreDraw("WireFrame");
	demoline.Draw();
}

void DemoScene::End()
{

}
