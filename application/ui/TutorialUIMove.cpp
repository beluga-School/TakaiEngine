#include "TutorialUIMove.h"
#include <GameUIManager.h>

void TutorialUIMove::Initialize()
{
}

void TutorialUIMove::Update()
{
}

void TutorialUIMove::Draw()
{
}

void TutorialUIMove::Start()
{
	GameUIManager::Get()->Move(UIMove::START, "TutorialMove");
}

void TutorialUIMove::End()
{
	GameUIManager::Get()->Move(UIMove::END, "TutorialMove");
}

bool TutorialUIMove::EndFlag()
{
	return false;
}
