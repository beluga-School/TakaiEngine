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

	testSphere.Initialize();
	testSphere.SetModel(ModelManager::GetModel("smSphere"));
	testSphere.SetTexture(TextureManager::GetTexture("slime"));
	skydome.Initialize();
}

GUI lightGUI("lightOperator");

void ShaderTestScene::Update()
{
	lightGUI.Begin({ 100,100 }, { 500,100 });
	ImGui::Text("lightdir");
	ImGui::SliderFloat(":x",&lightdir.x,-10.0f,10.0f);
	ImGui::SliderFloat(":y",&lightdir.y,-10.0f,10.0f);
	ImGui::SliderFloat(":z",&lightdir.z,-10.0f,10.0f);
	lightGUI.End();

	LightGroup::sLightGroup->SetDirLightDir(0, lightdir);

	debugCamera.Update();

	skydome.Update();
	testSphere.Update(*Camera::sCamera);
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	skydome.Draw();

	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));
	testSphere.Draw();

}

void ShaderTestScene::End()
{
}
