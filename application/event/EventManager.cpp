#include "EventManager.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"
#include "Clear1.h"
#include "GoalSystem.h"
#include <EventCameraManager.h>
#include <GameUIManager.h>
#include <StageChanger.h>

void EventManager::LoadResource()
{
	
}

bool EventManager::Start(const std::string& startEventName)
{
	for (auto &Event : allEvents)
	{
		//線イベントが競合するなら
		if (Event->isUseEventLine)
		{
			if (GetLineEvent() != nullptr)
			{
				//新しく入ってきた方は拒否
				if (GetLineEvent()->get()->isUseEventLine)return false;
			}
		}

		//イベントカメラなら複数実行を防ぐためのフラグを立てる
		if (Event->eventName == "startCamera")
		{
			if (stage1CamFlag)
			{
				break;
			}
			if (StageChanger::Get()->GetNowStageHandle() == "stage_mountain")
			{
				stage1CamFlag = true;
			}
		}

		//イベントがあれば実行
		if (Event->eventName == startEventName)
		{
			startTimer.Start();
			
			//多分コピーできねえって怒られる(ポインタ渡してるだけだから問題ないかも？)
			runEvents.push_back(&Event);
			Event->Initialize();
			Event->state = IEvent::EventState::Start;

			bool result = EventCameraManager::Get()->SetEventCamera(Event->eventName);

			//イベントカメラが見つかったら
			if (result)
			{
				//カメラ使うフラグを立てる
				Event->isUseCamera = true;
			}

			return true;
		}
	}
	//無ければ実行しない
	return false;
}

void EventManager::ForceEnd(const std::string endEventName)
{
	////イベント名が指定されているなら、現在実行中イベントを確認する
	//if (endEventName != "")
	//{
	//	if (GetNowEvent() == nullptr)return;
	//	//イベント名が一致しないなら通さない
	//	if (GetNowEvent()->get()->eventName != endEventName)return;
	//}

	if (GetRunEvent(endEventName) == nullptr)return;

	GetRunEvent(endEventName)->get()->isExecuted = true;
	DeleteRunEvent(endEventName);

	uppos.x = 0;
	downpos.x = Util::WIN_WIDTH;
}

void EventManager::End(const std::string endEventName)
{
	////イベント名が指定されているなら、現在実行中イベントを確認する
	//if (endEventName != "")
	//{
	//	if (GetNowEvent() == nullptr)return;
	//	//イベント名が一致しないなら通さない
	//	if (GetNowEvent()->get()->eventName != endEventName)return;
	//}

	if (GetRunEvent(endEventName) == nullptr)return;

	endTimer.Start();
	GetRunEvent(endEventName)->get()->End();
	DeleteRunEvent(endEventName);
}

std::unique_ptr<IEvent>* EventManager::GetLineEvent()
{
	for (auto& Event : runEvents)
	{
		//イベントがあれば返す
		if (Event->get()->isUseEventLine)
		{
			return Event;
		}
	}
	return nullptr;
}

std::unique_ptr<IEvent>* EventManager::CheckExestEvent(const std::string& eventName)
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

std::unique_ptr<IEvent>* EventManager::GetRunEvent(const std::string& eventName)
{
	for (auto& Event : runEvents)
	{
		//イベントがあれば返す
		if (Event->get()->eventName == eventName)
		{
			return Event;
		}
	}
	return nullptr;
}

void EventManager::DeleteRunEvent(const std::string& eventName)
{
	for (auto itr = runEvents.begin(); itr != runEvents.end();)
	{
		IEvent* e = (*itr)->get();
		
		if (e->eventName == eventName)
		{
			itr = runEvents.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	//for (auto& Event : runEvents)
	//{
	//	//イベントがあれば消す
	//	if (Event->get()->eventName == eventName)
	//	{
	//		Event->reset();
	//	}
	//}
}

void EventManager::Initialize()
{
	uppos = { 0,0 };
	downpos = { Util::WIN_WIDTH,Util::WIN_HEIGHT };
}

void EventManager::Update()
{
	//if (nowEvent == nullptr)return;

	startTimer.Update();
	endTimer.Update();

	for (auto& Event : runEvents)
	{
		if (runEvents.size() <= 0)break;

		switch (Event->get()->state)
		{
		case IEvent::EventState::None:

			break;
		case IEvent::EventState::Start:
			if (Event->get()->isUseEventLine)
			{
				uppos.x = TEasing::OutQuad(0, Util::WIN_WIDTH, startTimer.GetTimeRate());
				downpos.x = TEasing::OutQuad(Util::WIN_WIDTH, 0, startTimer.GetTimeRate());
				if (startTimer.GetEnd())
				{
					Event->get()->Start();
					Event->get()->state = IEvent::EventState::RunEvent;
					EventCameraManager::Get()->Start();
				}
			}
			else
			{
				Event->get()->Start();
				Event->get()->state = IEvent::EventState::RunEvent;
			}

			break;
		case IEvent::EventState::RunEvent:
			//イベント発生
			Event->get()->Update();

			//イベント終了
			if (Event->get()->EndFlag())
			{
				End();
				Event->get()->state = IEvent::EventState::End;
			}

			break;
		case IEvent::EventState::End:
			if (Event->get()->isUseEventLine)
			{
				uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
				downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
				if (endTimer.GetEnd())
				{
					DeleteRunEvent(Event->get()->eventName);
				}
			}
			break;
		}
	}
}

void EventManager::Draw()
{
	for (auto& Event : runEvents)
	{
		if (Event->get()->isUseEventLine)
		{
			InstantDrawer::DrawBox(uppos.x, uppos.y, Util::WIN_WIDTH, 100, Color(0, 0, 0, 1),
				InstantDrawer::Anchor::RIGHT);
			InstantDrawer::DrawBox(downpos.x, downpos.y, Util::WIN_WIDTH, 100, Color(0, 0, 0, 1),
				InstantDrawer::Anchor::LEFT);
			
			
		}

		if (Event->get()->state == IEvent::EventState::RunEvent)
		{
			Event->get()->Draw();
		}
	}
}

void EventManager::Clear()
{
	EventManager::Get()->runEvents.clear();
	EventManager::Get()->allEvents.clear();
	GameUIManager::Get()->Initialize();
}

void EventManager::CamFlagReset()
{
	stage1CamFlag = false;
}
