#include "GameUIManager.h"
#include "Util.h"

void GameUIManager::LoadResource()
{
	StageTitleUI::LoadResource();
	TextureManager::Load("Resources\\ui\\eyeMoveUI.png", "eyeMoveUI");
	TextureManager::Load("Resources\\ui\\jumpUI.png", "jumpUI");
	TextureManager::Load("Resources\\ui\\moveUI.png", "moveUI");
	TextureManager::Load("Resources\\ui\\moveUI.png", "moveUI");
	TextureManager::Load("Resources\\stagename_1.png", "stageTitle_1");
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
	if (handle == "stageTitleUI_mountain")
	{
		selectTitleUIMountain.Move(uimove);
	}
}

void GameUIManager::Reset()
{
	tutorialEyeMove.Initialize();
	tutorialJump.Initialize();
	tutorialMove.Initialize();
	selectTitleUIMountain.Initialize();
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
	
	selectTitleUIMountain.SetTexture("stageTitle_1");
	selectTitleUIMountain.InitPos({ (float)Util::CenterX(),-200 }, { (float)Util::CenterX(),50 });
	selectTitleUIMountain.InitScale({ 1,1 }, { 1,1 });
	selectTitleUIMountain.SetSize({ 1,1 });
}

void GameUIManager::Update()
{
	stageTitleUI.Update();
	tutorialEyeMove.Update();
	tutorialJump.Update();
	tutorialMove.Update();
	selectTitleUIMountain.Update();
}

void GameUIManager::Draw()
{
	stageTitleUI.Draw();
	tutorialEyeMove.Draw();
	tutorialJump.Draw();
	tutorialMove.Draw();
	selectTitleUIMountain.Draw();
}
