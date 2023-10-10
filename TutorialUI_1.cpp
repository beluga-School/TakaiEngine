#include "TutorialUI_1.h"
#include "GameUIManager.h"

void TutorialUI_1::Initialize()
{
	
}

void TutorialUI_1::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "TutorialJump");
}

void TutorialUI_1::End()
{
	GameUIManager::Get()->Move(UIMove::END, "TutorialJump");
}

void TutorialUI_1::Update()
{

}

void TutorialUI_1::Draw()
{

}

bool TutorialUI_1::EndFlag()
{
	return false;
}
