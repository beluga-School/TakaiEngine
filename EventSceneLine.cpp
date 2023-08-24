#include "EventSceneLine.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"

void EventSceneLine::Start()
{
	startTimer.Start();
	state = State::Start;
}

void EventSceneLine::End()
{
	endTimer.Start();
	state = State::End;
}

void EventSceneLine::Initialize()
{
	uppos = { 0,0 };
	downpos = { Util::WIN_WIDTH,Util::WIN_HEIGHT };
}

void EventSceneLine::Update()
{
	if (Input::Keyboard::TriggerKey(DIK_V))
	{
		switch (state)
		{
		case EventSceneLine::State::None:
			Start();
			break;
		case EventSceneLine::State::Start:
			End();
			break;
		}
	}

	startTimer.Update();
	endTimer.Update();

	switch (state)
	{
	case EventSceneLine::State::None:

		break;
	case EventSceneLine::State::Start:
		uppos.x = TEasing::OutQuad(0, Util::WIN_WIDTH, startTimer.GetTimeRate());
		downpos.x = TEasing::OutQuad(Util::WIN_WIDTH, 0, startTimer.GetTimeRate());
		break;
	case EventSceneLine::State::End:
		uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
		downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
		if (endTimer.GetEnd())
		{
			state = State::None;
		}

		break;
	}
}

void EventSceneLine::Draw()
{
	InstantDrawer::DrawBox(uppos.x, uppos.y, Util::WIN_WIDTH,100,Color(0,0,0,1),
		InstantDrawer::Anchor::RIGHT);
	InstantDrawer::DrawBox(downpos.x, downpos.y, Util::WIN_WIDTH,100,Color(0,0,0,1), 
		InstantDrawer::Anchor::LEFT);
}