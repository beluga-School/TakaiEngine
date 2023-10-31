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
	skydome.Initialize();
}

void DemoScene::Update()
{
	mDebugCamera.Update();
	demoline.Update(*Camera::sCamera);

	skydome.Update();
}

void DemoScene::Draw()
{
	skydome.Draw();

	BasicObjectPreDraw("Toon");
	demoline.Draw();
}

void DemoScene::End()
{

}
