#include "EventManager.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"

TEasing::easeTimer EventManager::startTimer = 0.5f;
TEasing::easeTimer EventManager::endTimer = 0.5f;

Vector2 EventManager::uppos{};
Vector2 EventManager::downpos{};

EventManager::State EventManager::state = State::None;

bool EventManager::Start(const std::string& startEventName)
{
	for (auto &Event : EventManager::Get()->allEvents)
	{
		//イベントがあれば実行
		if (Event == startEventName)
		{
			startTimer.Start();
			state = State::Start;
			EventManager::Get()->nowEvent = startEventName;
			return true;
		}
	}
	//無ければ実行しない
	return false;
}

void EventManager::End()
{
	endTimer.Start();
	state = State::End;
}

std::string EventManager::GetNowEvent()
{
	return nowEvent;
}

void EventManager::Initialize()
{
	uppos = { 0,0 };
	downpos = { Util::WIN_WIDTH,Util::WIN_HEIGHT };
}

void EventManager::Update()
{
	startTimer.Update();
	endTimer.Update();

	switch (state)
	{
	case EventManager::State::None:

		break;
	case EventManager::State::Start:
		uppos.x = TEasing::OutQuad(0, Util::WIN_WIDTH, startTimer.GetTimeRate());
		downpos.x = TEasing::OutQuad(Util::WIN_WIDTH, 0, startTimer.GetTimeRate());
		break;
	case EventManager::State::End:
		uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
		downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
		if (endTimer.GetEnd())
		{
			state = State::None;
			EventManager::Get()->nowEvent = "";
		}

		break;
	}
}

void EventManager::Draw()
{
	InstantDrawer::DrawBox(uppos.x, uppos.y, Util::WIN_WIDTH,100,Color(0,0,0,1),
		InstantDrawer::Anchor::RIGHT);
	InstantDrawer::DrawBox(downpos.x, downpos.y, Util::WIN_WIDTH,100,Color(0,0,0,1), 
		InstantDrawer::Anchor::LEFT);
}

void EventManager::Register(const std::string& startEventName)
{
	EventManager::Get()->allEvents.push_back(startEventName);
}

void EventManager::Clear()
{
	EventManager::Get()->allEvents.clear();
}
