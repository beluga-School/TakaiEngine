#include "StageTitleUIMountain.h"
#include "GameUIManager.h"

void StageTitleUIMountain::Initialize()
{

}

void StageTitleUIMountain::Update()
{

}

void StageTitleUIMountain::Draw()
{

}

void StageTitleUIMountain::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "stageTitleUI_mountain");
}

void StageTitleUIMountain::End()
{
	GameUIManager::Get()->Move(UIMove::END, "stageTitleUI_mountain");
}

bool StageTitleUIMountain::EndFlag()
{
	return false;
}
