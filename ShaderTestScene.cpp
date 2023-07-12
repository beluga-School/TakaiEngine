#include "ShaderTestScene.h"
#include "ClearDrawScreen.h"
#include "LightGroup.h"
#include "Input.h"
#include "ImguiManager.h"

void ShaderTestScene::LoadResource()
{
}

void ShaderTestScene::Initialize()
{
	debugCamera.Initialize();

	skydome.Initialize();

	billboard.Initialize();
	billboard.position = {0,0,0};
	billboard.scale = {3,3,3};
}

GUI lightGUI("lightOperator");

void ShaderTestScene::Update()
{
	debugCamera.Update();

	skydome.Update();

	billboard.Update(*Camera::sCamera);
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	skydome.Draw();

	//BasicObjectPreDraw(PipelineManager::GetPipeLine("Phong"));
	//billboard.Draw();
	BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"),false);
	billboard.Draw();
}

void ShaderTestScene::End()
{
}
