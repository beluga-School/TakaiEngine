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

	lineObject.Initialize();
	lineObject.SetModel(ModelManager::GetModel("ICOSphere"));
	lineObject.SetTexture(TextureManager::GetTexture("white"));
}

GUI lightGUI("lightOperator");

void ShaderTestScene::Update()
{
	debugCamera.Update();

	skydome.Update();

	lineObject.Update(*Camera::sCamera);
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	skydome.Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("WireFrame"));
	lineObject.Draw();
}

void ShaderTestScene::End()
{
}
