#include "GameUIManager.h"
#include "Util.h"

void GameUIManager::LoadResource()
{
	StageTitleUI::LoadResource();
	TextureManager::Load("Resources\\ui\\eyeMoveUI.png", "eyeMoveUI");
}

void GameUIManager::Move(UIMove uimove, const std::string& handle)
{
	if (handle == "StageTitle")
	{
		stageTitleUI.Start();
	}
	if (handle == "TutorialJump")
	{
		tutorial1.Move(uimove);
	}
}

void GameUIManager::Initialize()
{
	stageTitleUI.Initialize();

	tutorial1.SetTexture("eyeMoveUI");
	tutorial1.Initialize();
	tutorial1.InitPos({ (float)Util::CenterX(),Util::WIN_HEIGHT + 200 },{ (float)Util::CenterX(),Util::WIN_HEIGHT });
	tutorial1.InitScale({ 0.5f,0.5f }, { 0.5f,0.5f });
	tutorial1.SetSize({ 0.5f,0.5f });
}

void GameUIManager::Update()
{
	stageTitleUI.Update();
	tutorial1.Update();
}

void GameUIManager::Draw()
{
	stageTitleUI.Draw();
	tutorial1.Draw();
}
