#pragma once
#include "UI.h"
#include "TEasing.h"

class StarUI : public UI
{
public:
	StarUI(const Vector2& pos, float scale) : UI(pos,scale)
	{

	}

	enum class GetMoveState
	{
		None,
		CenterMove,
		CountUp,
		BackMove,
		End,
	};

	void Update();

	//���l��������Ƃ��̓������܂Ƃ߂�����
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

