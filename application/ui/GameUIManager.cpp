#include "GameUIManager.h"
#include "Util.h"

void GameUIManager::LoadResource()
{
	StageTitleUI::LoadResource();
	StarUI::LoadResource();
	TextureManager::Load("Resources\\ui\\eyeMoveUI.png", "eyeMoveUI");
	TextureManager::Load("Resources\\ui\\jumpUI.png", "jumpUI");
	TextureManager::Load("Resources\\ui\\moveUI.png", "moveUI");
	TextureManager::Load("Resources\\ui\\moveUI.png", "moveUI");
	TextureManager::Load("Resources\\stagename_1.png", "stageTitle_1");
	TextureManager::Load("Resources\\ui\\enemyDownUI.png", "enemyDownUI");
	TextureManager::Load("Resources\\ui\\getstarUI.png", "getstarUI");
	TextureManager::Load("Resources\\ui\\inDokanUI.png", "inDokanUI");
}

void GameUIManager::Move(UIMove uimove, const std::string& handle)
{
	if (handle == "StageTitle")
	{
		stageTitleUI.Start();
	}
	if (handle == "tutorialUI_EyeMove")
	{
		tutorialEyeMove.Move(uimove);
	}
	if (handle == "tutorialUI_Jump")
	{
		tutorialJump.Move(uimove);
	}
	if (handle == "tutorialUI_Move")
	{
		tutorialMove.Move(uimove);
	}
	if (handle == "stageTitleUI_mountain")
	{
		selectTitleUIMountain.Move(uimove);
	}
	if (handle == "tutorialUI_EnemyDown")
	{
		tutorialEnemyDown.Move(uimove);
	}
	if (handle == "tutorialUI_GetStar")
	{
		tutorialGetStar.Move(uimove);
		//スター取得チュートリアルが出たら一緒にスターのUIも出す
		starUI.Move(UIMove::START);
		//出っぱなしフラグを立てる
		starUI.uiAppearance = true;
	}
	if (handle == "tutorialUI_InDokan")
	{
		tutorialInDokan.Move(uimove);
	}
}

void GameUIManager::Reset()
{
	tutorialEyeMove.Initialize();
	tutorialJump.Initialize();
	tutorialMove.Initialize();
	selectTitleUIMountain.Initialize();
	tutorialEnemyDown.Initialize();
	tutorialGetStar.Initialize();
	tutorialInDokan.Initialize();

	//スターが置かれるたびにカウントアップするのでここで初期化
	starUI.Substitution(0);
}

void GameUIManager::Initialize()
{
	stageTitleUI.Initialize();
	starUI.Initialize();

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

	tutorialEnemyDown.SetTexture("enemyDownUI");
	tutorialEnemyDown.InitPos({ 300,-300 }, { 300,200 });
	tutorialEnemyDown.InitScale({ 1,1 }, { 1,1 });
	tutorialEnemyDown.SetSize({ 1,1 });
	
	tutorialGetStar.SetTexture("getstarUI");
	tutorialGetStar.InitPos({ 300,-300 }, { 300,200 });
	tutorialGetStar.InitScale({ 1,1 }, { 1,1 });
	tutorialGetStar.SetSize({ 1,1 });
	
	tutorialInDokan.SetTexture("inDokanUI");
	tutorialInDokan.InitPos({ 300,-300 }, { 300,200 });
	tutorialInDokan.InitScale({ 1,1 }, { 1,1 });
	tutorialInDokan.SetSize({ 1,1 });
}

void GameUIManager::Update()
{
	stageTitleUI.Update();
	starUI.Update();

	tutorialEyeMove.Update();
	tutorialJump.Update();
	tutorialMove.Update();
	selectTitleUIMountain.Update();
	tutorialEnemyDown.Update();
	tutorialGetStar.Update();
	tutorialInDokan.Update();
}

void GameUIManager::Draw()
{
	stageTitleUI.Draw();
	starUI.Draw();

	tutorialEyeMove.Draw();
	tutorialJump.Draw();
	tutorialMove.Draw();
	selectTitleUIMountain.Draw();
	tutorialEnemyDown.Draw();
	tutorialGetStar.Draw();
	tutorialInDokan.Draw();
}
