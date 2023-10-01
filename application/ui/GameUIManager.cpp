#include "GameUIManager.h"

void GameUIManager::LoadResource()
{
	StarUI::LoadResource();
	StageTitleUI::LoadResource();
}

void GameUIManager::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		stageTitleUI.Start();
		break;
	case UIMove::END:
		break;
	}
	starUI.Move(uimove);
}

void GameUIManager::Initialize()
{
	starUI.Initialize();
	stageTitleUI.Initialize();
}

void GameUIManager::Update()
{
	starUI.Update();
	stageTitleUI.Update();
}

void GameUIManager::Draw()
{
	starUI.Draw();
	stageTitleUI.Draw();
}
