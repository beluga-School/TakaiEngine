#include "ShaderTestScene.h"
#include "ClearDrawScreen.h"
#include "LightGroup.h"
#include "Input.h"
#include "ImguiManager.h"
#include "TimeManager.h"

void ShaderTestScene::LoadResource()
{
}

void ShaderTestScene::Initialize()
{
	debugCamera.Initialize();

	skydome.Initialize();

	/*billboard.Initialize();
	billboard.position = {0,0,0};
	billboard.scale = {3,3,3};*/

	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Dirt.png", "Dirt");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\FirldMask.png", "FirldMask");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Grass.png", "Grass");

	testObj.Initialize();
	testObj.SetTexture(TextureManager::GetTexture("Grass"));
	sub = TextureManager::GetTexture("Dirt");
	mask = TextureManager::GetTexture("FirldMask");
}

GUI lightGUI("lightOperator");

void ShaderTestScene::Update()
{
	debugCamera.Update();

	skydome.Update();

	testObj.Update(*Camera::sCamera);
	//billboard.Update(*Camera::sCamera);
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	skydome.Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("TextureBlend"));
	testObj.DrawBlendTexture(*sub,*mask);
	/*BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"),false);
	billboard.Draw();*/
}

void ShaderTestScene::End()
{
}
