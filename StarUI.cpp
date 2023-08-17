#include "StarUI.h"
#include "Util.h"

void StarUI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\star2d.png", "star2d");
}

void StarUI::Update()
{
	GetMove();

	UI::Update();
}

void StarUI::GetMove()
{
	Vector2 endPos = {
		Util::WIN_WIDTH / 2,
		Util::WIN_HEIGHT / 2 - 200,
	};

	moveTimer.Update();
	countUpTimer.Update();
	
	if (getMoveState != GetMoveState::None)
	{
		AppLock();
	}

	switch (getMoveState)
	{
	case StarUI::GetMoveState::None:
		
		break;
	case StarUI::GetMoveState::CenterMove:
		mPos.x = TEasing::OutExpo(saveStartPos.x, endPos.x, moveTimer.GetTimeRate());
		mPos.y = TEasing::OutExpo(saveStartPos.y, endPos.y, moveTimer.GetTimeRate());

		if (moveTimer.GetEnd())
		{
			moveTimer.Reset();
			
			countUpTimer.Start();

			getMoveState = GetMoveState::CountUp;
		}
		break;
	case StarUI::GetMoveState::CountUp:
		mPos.y = TEasing::InQuad(endPos.y, endPos.y - 5, countUpTimer.GetTimeRate());
		
		mScale.x = TEasing::InQuad(saveScale.x, saveScale.x * 1.2f, countUpTimer.GetTimeRate());
		mScale.y = TEasing::InQuad(saveScale.y, saveScale.y * 1.2f, countUpTimer.GetTimeRate());

		if (countUpTimer.GetEnd())
		{
			countUpTimer.ReverseStart();
			UpdateNumber(saveStatus);
		}
		
		if (countUpTimer.GetReverseEnd())
		{
			getMoveState = GetMoveState::BackMove;
			moveTimer.Start();
		}
		break;
	case StarUI::GetMoveState::BackMove:

		mPos.x = TEasing::InExpo(endPos.x, saveStartPos.x, moveTimer.GetTimeRate());
		mPos.y = TEasing::InExpo(endPos.y, saveStartPos.y, moveTimer.GetTimeRate());

		if (moveTimer.GetEnd())
		{
			moveTimer.Reset();
			getMoveState = GetMoveState::End;
		}

		break;
	case StarUI::GetMoveState::End:
		getMoveState = GetMoveState::None;
		break;
	}
}

void StarUI::GetMoveStart(int32_t status)
{
	if (getMoveState != GetMoveState::None)return;
	saveStatus = status;
	saveStartPos = mPos;

	saveScale = mScale;

	moveTimer.Start();
	countUpTimer.Reset();
	
	getMoveState = GetMoveState::CenterMove;
}
