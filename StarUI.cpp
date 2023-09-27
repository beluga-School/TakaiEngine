#include "StarUI.h"
#include "Util.h"

void StarUI::LoadResource()
{
	TextureManager::Load("Resources\\UI\\star2d.png", "star2d");
	TextureManager::Load("Resources\\UI\\starUi.png", "starUi");
}

void StarUI::Initialize()
{
	uibase.SetTexture("starUi");
	
	uibase.InitPos({ (float)Util::WIN_WIDTH + 200,100 }, { (float)Util::WIN_WIDTH - 200,100 });
	uibase.InitScale({ 0.5f,0.5f }, { 1.0f,1.0f });
}

void StarUI::Update()
{
	if (uibase.GetMoveEnd(UIMove::END))
	{
		uibase.Scaling(UIMove::START);
	}
	uibase.Update();
}

void StarUI::Draw()
{
	uibase.Draw();
}

void StarUI::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		uibase.Move(uimove);
		break;
	case UIMove::END:
		uibase.Move(uimove);
		break;
	}
}
