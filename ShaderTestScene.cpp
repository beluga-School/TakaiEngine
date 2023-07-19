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
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\DissolveMap.png", "DissolveMap");

	testObj.Initialize();
	testObj.SetTexture(TextureManager::GetTexture("Grass"));
	sub = TextureManager::GetTexture("Dirt");
	blendMask = TextureManager::GetTexture("FirldMask");
	disolveMask = TextureManager::GetTexture("DissolveMap");
}

GUI lightGUI("lightOperator");

void ShaderTestScene::Update()
{
	debugCamera.Update();

	skydome.Update();

	testObj.Update(*Camera::sCamera);
	//billboard.Update(*Camera::sCamera);

	gui.Begin({ 100,100 }, { 200,200 });
	if (ImGui::Button("TextureBlend"))
	{
		mode = DrawMode::TextureBlend;
	}
	if (ImGui::Button("Disolve"))
	{
		mode = DrawMode::Disolve;
	}
	ImGui::SliderFloat("disolveValue", &testObj.disolveVal,0.0f,1.0f);
	gui.End();
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Skydome"));
	skydome.Draw();

	switch (mode)
	{
	case ShaderTestScene::DrawMode::TextureBlend:
		BasicObjectPreDraw(PipelineManager::GetPipeLine("TextureBlend"));
		//testObj.DrawSpecial(SpecialDraw::TEXTUREBLEND_,*sub, *blendMask);
		break;
	case ShaderTestScene::DrawMode::Disolve:
		BasicObjectPreDraw(PipelineManager::GetPipeLine("Disolve"));
		//testObj.DrawSpecial(SpecialDraw::DISOLVE_ ,*disolveMask);
		break;
	default:
		break;
	}

	/*BasicObjectPreDraw(PipelineManager::GetPipeLine("PerlinNoise"),false);
	billboard.Draw();*/
}

void ShaderTestScene::End()
{
}
