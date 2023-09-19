#pragma once
#include "IStarUI.h"
#include "TEasing.h"

class StarUI : public IStarUI
{
public:
	StarUI(const Vector2& pos, float scale) : IStarUI(pos,scale)
	{

	}

	void LoadResource();

	enum class GetMoveState
	{
		None,
		CenterMove,
		CountUp,
		BackMove,
		End,
	};

	void Update();

	//”’l‚ª‘‚¦‚é‚Æ‚«‚Ì“®‚«‚ğ‚Ü‚Æ‚ß‚½‚à‚Ì
	void GetMove();

	void GetMoveStart(int32_t status);

private:
	GetMoveState getMoveState = GetMoveState::None;

	Color backColor = { 60.f / 255.f,78.f / 255.f,54.f / 255.f,1.f };

	Vector2 saveStartPos = {};
	Vector2 saveScale = {};
	int32_t saveStatus = 0;

	TEasing::easeTimer moveTimer = 0.5f;

	TEasing::easeTimer countUpTimer = 0.05f;
};

