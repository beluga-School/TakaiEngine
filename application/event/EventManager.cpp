#include "EventManager.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"
#include "Clear1.h"
#include "GoalSystem.h"
#include <EventCameraManager.h>
#include <GameUIManager.h>

void EventManager::LoadResource()
{
	
}

bool EventManager::Start(const std::string& startEventName)
{
	//イベントが入っていれば成功で返す
	if (GetNowEvent() != nullptr)return true;

	for (auto &Event : allEvents)
	{
		//イベントがあれば実行
		if (Event->eventName == startEventName)
		{
			startTimer.Start();
			state = State::Start;
			
			nowEvent = &Event;
			Event->Initialize();
			
			bool result = EventCameraManager::Get()->SetEventCamera(Event->eventName);

			//イベントカメラが見つかったら
			if (result)
			{
				//カメラ使うフラグを立てる
				nowEvent->get()->isUseCamera = true;
			}

			return true;
		}
	}
	//無ければ実行しない
	return false;
}

void EventManager::ForceEnd(const std::string endEventName)
{
	//イベント名が指定されているなら、現在実行中イベントを確認する
	if (endEventName != "")
	{
		if (GetNowEvent() == nullptr)return;
		//イベント名が一致しないなら通さない
		if (GetNowEvent()->get()->eventName != endEventName)return;
	}

	state = State::None;
	nowEvent->get()->isExecuted = true;
	nowEvent = nullptr;

	uppos.x = 0;
	downpos.x = Util::WIN_WIDTH;
}

void EventManager::End(const std::string endEventName)
{
	//イベント名が指定されているなら、現在実行中イベントを確認する
	if (endEventName != "")
	{
		if (GetNowEvent() == nullptr)return;
		//イベント名が一致しないなら通さない
		if (GetNowEvent()->get()->eventName != endEventName)return;
	}

	endTimer.Start();
	state = State::End;
	nowEvent->get()->End();
}

std::unique_ptr<IEvent>* EventManager::GetNowEvent()
{
	return nowEvent;
}

std::unique_ptr<IEvent>* EventManager::GetEvent(const std::string& eventName)
{
	for (auto& Event : allEvents)
	{
		//イベントがあれば返す
		if (Event->eventName == eventName)
		{
			return &Event;
		}
	}

	//見つからなかったらぬるぽ
	return nullptr;
}

void EventManager::Initialize()
{
	uppos = { 0,0 };
	downpos = { Util::WIN_WIDTH,Util::WIN_HEIGHT };
}

void EventManager::Update()
{
	if (nowEvent == nullptr)return;

	startTimer.Update();
	endTimer.Update();

	switch (state)
	{
	case EventManager::State::None:

		break;
	case EventManager::State::Start:
		uppos.x = TEasing::OutQuad(0, Util::WIN_WIDTH, startTimer.GetTimeRate());
		downpos.x = TEasing::OutQuad(Util::WIN_WIDTH, 0, startTimer.GetTimeRate());
		if (startTimer.GetEnd())
		{
			nowEvent->get()->Start();
			state = State::RunEvent;
			EventCameraManager::Get()->Start();
		}
		
		break;
	case EventManager::State::RunEvent:
		//イベント発生
		nowEvent->get()->Update();

		//イベント終了
		if(nowEvent->get()->EndFlag())
		{
			End();
		}

		break;
	case EventManager::State::End:
		uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
		downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
		if (endTimer.GetEnd())
		{
			//強制終了と同じ効果を呼び出す
			ForceEnd();
		}
		break;
	}
}

void EventManager::Draw()
{
	if (nowEvent != nullptr)
	{
		if (nowEvent->get()->isUseEventLine)
		{
			InstantDrawer::DrawBox(uppos.x, uppos.y, Util::WIN_WIDTH, 100, Color(0, 0, 0, 1),
				InstantDrawer::Anchor::RIGHT);
			InstantDrawer::DrawBox(downpos.x, downpos.y, Util::WIN_WIDTH, 100, Color(0, 0, 0, 1),
				InstantDrawer::Anchor::LEFT);
		}
		if (state == State::RunEvent)
		{
			nowEvent->get()->Draw();
		}
	}
}

void EventManager::Clear()
{
	EventManager::Get()->allEvents.clear();
	EventManager::Get()->nowEvent = nullptr;
	GameUIManager::Get()->Reset();
}