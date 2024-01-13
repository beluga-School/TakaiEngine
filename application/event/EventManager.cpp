#include "EventManager.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"
#include "Clear1.h"
#include "GoalSystem.h"
#include <EventCameraManager.h>
#include <StageChanger.h>
#include "GameUIManager.h"

void EventManager::LoadResource()
{
	TextureManager::Load("Resources\\ui\\slaystring.png", "slaystring");
	TextureManager::Load("Resources\\ui\\trapstring.png", "trapstring");
	TextureManager::Load("Resources\\ui\\getstarstring.png", "getstarstring");
	TextureManager::Load("Resources\\ui\\redcoinString.png", "redcoinString");
	TextureManager::Load("Resources\\ui\\clearString.png", "clearString");
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
	if (CheckRunEvent(endEventName) == nullptr)return;

	CheckRunEvent(endEventName)->get()->isExecuted = true;
	DeleteRunEvent(endEventName);

	uppos.x = 0;
	downpos.x = Util::WIN_WIDTH;
}

void EventManager::End(const std::string endEventName)
{
	if (CheckRunEvent(endEventName) == nullptr)return;

	endTimer.Start();
	CheckRunEvent(endEventName)->get()->isExecuted = true;
	CheckRunEvent(endEventName)->get()->End();
	
	if (!CheckRunEvent(endEventName)->get()->isUseEventLine) {
		DeleteRunEvent(endEventName);
	}
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

std::unique_ptr<IEvent>* EventManager::CheckRunEvent(const std::string& eventName)
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

std::list<std::unique_ptr<IEvent>*> EventManager::GetNowEvents()
{
	return runEvents;
}

bool EventManager::DeleteRunEvent(const std::string& eventName)
{
	for (auto itr = runEvents.begin(); itr != runEvents.end();)
	{
		IEvent* e = (*itr)->get();

		if (e->eventName == eventName)
		{
			itr = runEvents.erase(itr);
			deleteCheck = true;
			return true;
		}
		else
		{
			itr++;
		}
	}

	return false;
}

void EventManager::PreRegister()
{
	Register<ClearEvent>("clearEvent");
}

void EventManager::Initialize()
{
	uppos = { 0,0 };
	downpos = { Util::WIN_WIDTH,Util::WIN_HEIGHT };

	PreRegister();
}

void EventManager::Update()
{
	startTimer.Update();
	endTimer.Update();

	deleteCheck = false;

	for (auto& Event : runEvents)
	{
		if (deleteCheck)break;

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
				Event->get()->state = IEvent::EventState::End;
				End(Event->get()->eventName);
			}

			break;
		case IEvent::EventState::End:
			if (Event->get()->isUseEventLine)
			{
				uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
				downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
				if (endTimer.GetEnd())
				{
					deleteCheck = DeleteRunEvent(Event->get()->eventName);
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
