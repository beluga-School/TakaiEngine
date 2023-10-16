#include "GameUIManager.h"
#include "Util.h"

void GameUIManager::LoadResource()
{
	StageTitleUI::LoadResource();
	TextureManager::Load("Resources\\ui\\eyeMoveUI.png", "eyeMoveUI");
	TextureManager::Load("Resources\\ui\\jumpUI.png", "jumpUI");
	TextureManager::Load("Resources\\ui\\moveUI.png", "moveUI");
}

void GameUIManager::Move(UIMove uimove, const std::string& handle)
{
	if (handle == "StageTitle")
	{
		stageTitleUI.Start();
	}
	if (handle == "TutorialEyeMove")
	{
		tutorialEyeMove.Move(uimove);
	}
	if (handle == "TutorialJump")
	{
		tutorialJump.Move(uimove);
	}
	if (handle == "TutorialMove")
	{
		tutorialMove.Move(uimove);
	}
}

void GameUIManager::Reset()
{
	tutorialEyeMove.Initialize();
	tutorialJump.Initialize();
	tutorialMove.Initialize();
}

void GameUIManager::Initialize()
{
	stageTitleUI.Initialize();

	Reset();

	tutorialEyeMove.SetTexture("eyeMoveUI");
	tutorialEyeMove.InitPos({ (float)Util::CenterX(),Util::WIN_HEIGHT + 200 },{ (float)Util::CenterX(),Util::WIN_HEIGHT });
	tutorialEyeMove.InitScale({ 0.5f,0.5f }, { 0.5f,0.5f });
	tutorialEyeMove.SetSize({ 0.5f,0.5f });

	tutorialJump.SetTexture("jumpUI");
	tutorialJump.InitPos({ (float)Util::CenterX(),Util::WIN_HEIGHT + 200 }, { (float)Util::CenterX(),Util::WIN_HEIGHT });
	tutorialJump.InitScale({ 0.5f,0.5f }, { 0.5f,0.5f });
	tutorialJump.SetSize({ 0.5f,0.5f });
	

	tutorialMove.SetTexture("moveUI");
	tutorialMove.InitPos({ (float)Util::CenterX(),Util::WIN_HEIGHT + 200 }, { (float)Util::CenterX(),Util::WIN_HEIGHT });
	tutorialMove.InitScale({ 0.5f,0.5f }, { 0.5f,0.5f });
	tutorialMove.SetSize({ 0.5f,0.5f });
}

void GameUIManager::Update()
{
	stageTitleUI.Update();
	tutorialEyeMove.Update();
	tutorialJump.Update();
	tutorialMove.Update();
}

void GameUIManager::Draw()
{
	stageTitleUI.Draw();
	tutorialEyeMove.Draw();
	tutorialJump.Draw();
	tutorialMove.Draw();
}
