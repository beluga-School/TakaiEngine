#include "TutorialUIEyeMove.h"
#include "GameUIManager.h"

void TutorialUIEyeMove::Initialize()
{
	
}

void TutorialUIEyeMove::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "TutorialEyeMove");
}

void TutorialUIEyeMove::End()
{
	GameUIManager::Get()->Move(UIMove::END, "TutorialEyeMove");
}

void TutorialUIEyeMove::Update()
{

}

void TutorialUIEyeMove::Draw()
{

}

bool TutorialUIEyeMove::EndFlag()
{
	return false;
}
