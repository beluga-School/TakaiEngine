#include "SummonUI.h"
#include "GameUIManager.h"

void SummonUI::Initialize()
{
}

void SummonUI::Update()
{
}

void SummonUI::Draw()
{
}

void SummonUI::Start()
{
	GameUIManager::Get()->Move(UIMove::START, eventName);
}

void SummonUI::End()
{
	GameUIManager::Get()->Move(UIMove::END, eventName);
}

bool SummonUI::EndFlag()
{
	return false;
}
