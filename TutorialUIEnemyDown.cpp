#include "TutorialUIEnemyDown.h"
#include "GameUIManager.h"

void TutorialUIEnemyDown::Initialize()
{

}

void TutorialUIEnemyDown::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "TutorialEnemyDown");
}

void TutorialUIEnemyDown::End()
{
	GameUIManager::Get()->Move(UIMove::END, "TutorialEnemyDown");
}

void TutorialUIEnemyDown::Update()
{

}

void TutorialUIEnemyDown::Draw()
{

}

bool TutorialUIEnemyDown::EndFlag()
{
	return false;
}
