#include "ShaderTestScene.h"
#include "ClearDrawScreen.h"
#include "LightGroup.h"
#include "Input.h"
#include "ImguiManager.h"
#include "TimeManager.h"
#include "MathF.h"
#include "PostEffect.h"

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
	billboard.rotation = {0,3.14f,0};

	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Dirt.png", "Dirt");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\FirldMask.png", "FirldMask");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\Grass.png", "Grass");
	TextureManager::Load("Resources\\09_AlphaMask_Resources\\DissolveMap.png", "DissolveMap");

	testObj.Initialize();
	testObj.SetTexture(TextureManager::GetTexture("Grass"));
	testObj.rotation = { 0,3.14f,0 };

	sub = TextureManager::GetTexture("Dirt");
	blendMask = TextureManager::GetTexture("FirldMask");
	disolveMask = TextureManager::GetTexture("DissolveMap");

	handles.push_back("TextureBlend");
	handles.push_back("Disolve");
	handles.push_back("Noise");

	debugCamera.SetRadius(4.f);
}

void ShaderTestScene::Update()
{
	debugCamera.Update();

	skydome.Update();

	testObj.Update(*Camera::sCamera);
	billboard.Update(*Camera::sCamera);

	gui.Begin({ 100,100 }, { 300,200 });

	//ハンドルが空でなければ
	if (!handles.empty())
	{
		//ハンドルの一覧をプルダウンで表示
		std::vector<const char*> temp;
		for (size_t i = 0; i < handles.size(); i++)
		{
			temp.push_back(handles[i].c_str());
		}
		static int32_t select = 0;
		ImGui::Combo("DrawMode", &select, &temp[0], (int32_t)handles.size());

		//切り替え用の名前に保存
		output = handles[select];
	}

	if (output == "TextureBlend")
	{
		mode = DrawMode::TextureBlend;
		testObj.SetTexture(TextureManager::GetTexture("Grass"));
	}
	if (output == "Disolve")
	{
		mode = DrawMode::Disolve;
		ImGui::SliderFloat("disolveValue", &testObj.disolveVal, 0.0f, 1.0f);
		testObj.SetTexture(TextureManager::GetTexture("Dirt"));
	}
	if (output == "Noise")
	{
		mode = DrawMode::Noise;
	}

	//ノイズエフェクト切り替え
	static bool mRotCheck = false;
	if (ImGui::Button("NoiseEffect"))
	{
		mRotCheck = !mRotCheck;
	}
	if (mRotCheck)
	{
		PostEffect::pipeLineName = "PerlinNoisePE";
	}
	else
	{
		PostEffect::pipeLineName = "None";
	}

	ImGui::Text("camradius %f", debugCamera.GetRadius());
	gui.End();
}

void ShaderTestScene::Draw()
{
	BasicObjectPreDraw("Skydome");
	skydome.Draw();

	switch (mode)
	{
	case ShaderTestScene::DrawMode::TextureBlend:
		BasicObjectPreDraw("TextureBlend");
		testObj.DrawSpecial(SpecialDraw::TEXTUREBLEND_,*sub, *blendMask);
		break;
	case ShaderTestScene::DrawMode::Disolve:
		BasicObjectPreDraw("Disolve");
		testObj.DrawSpecial(SpecialDraw::DISOLVE_ ,*disolveMask);
		break;
	case ShaderTestScene::DrawMode::Noise:
		BasicObjectPreDraw("PerlinNoise", false);
		billboard.Draw();
		break;
	default:
		break;
	}
}

void ShaderTestScene::End()
{
}
