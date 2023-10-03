#include "EventManager.h"
#include "InstantDrawer.h"
#include "Util.h"
#include "Input.h"
#include "Clear1.h"
#include "GoalSystem.h"
#include <EventCameraManager.h>

bool EventManager::Start(const std::string& startEventName)
{
	//�C�x���g�������Ă���ΐ����ŕԂ�
	if (GetNowEvent() != nullptr)return true;

	for (auto &Event : allEvents)
	{
		//�C�x���g������Ύ��s
		if (Event->eventName == startEventName)
		{
			startTimer.Start();
			state = State::Start;
			
			nowEvent = &Event;
			
			bool result = EventCameraManager::Get()->SetEventCamera(Event->eventName);

			//�C�x���g�J����������������
			if (result)
			{
				//�J�����g���t���O�𗧂Ă�
				nowEvent->get()->isUseCamera = true;
			}

			return true;
		}
	}
	//������Ύ��s���Ȃ�
	return false;
}

void EventManager::ForceEnd()
{
	state = State::None;
	nowEvent->get()->isExecuted = true;
	nowEvent = nullptr;

	uppos.x = 0;
	downpos.x = Util::WIN_WIDTH;
}

void EventManager::End()
{
	endTimer.Start();
	state = State::End;
}

std::unique_ptr<IEvent>* EventManager::GetNowEvent()
{
	return nowEvent;
}

std::unique_ptr<IEvent>* EventManager::GetEvent(const std::string& eventName)
{
	for (auto& Event : allEvents)
	{
		//�C�x���g������ΕԂ�
		if (Event->eventName == eventName)
		{
			return &Event;
		}
	}

	//������Ȃ�������ʂ��
	return nullptr;
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
		if (startTimer.GetEnd())
		{
			nowEvent->get()->Start();
			state = State::RunEvent;
			EventCameraManager::Get()->Start();
		}
		
		break;
	case EventManager::State::RunEvent:
		//�C�x���g����
		nowEvent->get()->Update();

		//�C�x���g�I��
		if(nowEvent->get()->End())
		{
			End();
		}

		break;
	case EventManager::State::End:
		uppos.x = TEasing::InQuad(Util::WIN_WIDTH, Util::WIN_WIDTH * 2.f, endTimer.GetTimeRate());
		downpos.x = TEasing::InQuad(0, Util::WIN_WIDTH * -1.f, endTimer.GetTimeRate());
		if (endTimer.GetEnd())
		{
			//�����I���Ɠ������ʂ��Ăяo��
			ForceEnd();
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

void EventManager::Clear()
{
	EventManager::Get()->allEvents.clear();
}