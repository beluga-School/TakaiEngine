#include "TutorialUIJump.h"
#include "GameUIManager.h"

void TutorialUIJump::Initialize()
{
}

void TutorialUIJump::Update()
{
	
}

void TutorialUIJump::Draw()
{
	
}

void TutorialUIJump::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "TutorialJump");
}

void TutorialUIJump::End()
{
	GameUIManager::Get()->Move(UIMove::END, "TutorialJump");
}

bool TutorialUIJump::EndFlag()
{
	return false;
}
