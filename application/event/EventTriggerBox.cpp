#include "EventTriggerBox.h"
#include <Collision.h>
#include <EventManager.h>

void EventTriggerBox::Initialize()
{
	Obj3d::Initialize();
}

void EventTriggerBox::Update()
{
	Obj3d::Update(*Camera::sCamera);

	//自身の持っているイベントが終了フラグを発動したら
	if (EventManager::Get()->CheckExestEvent(eventName_) != nullptr) {
		if (EventManager::Get()->CheckExestEvent(eventName_)->get()->state == IEvent::EventState::End) {
			//終了する
			EventEnd();
		}
	}
}

void EventTriggerBox::Draw()
{
	Obj3d::Draw();
}

void EventTriggerBox::EventStart()
{
	//トリガーイベントで、すでに実行済みなら開始しない
	if (EventManager::Get()->CheckExestEvent(eventName_) != nullptr) {
		if (EventManager::Get()->CheckExestEvent(eventName_)->get()->isTrigerEvent && 
			EventManager::Get()->CheckExestEvent(eventName_)->get()->isExecuted)
		{
			return;
		}
	}

	if (trigger)
	{
		return;
	}
	EventManager::Get()->Start(eventName_);
	
	trigger = true;
}

void EventTriggerBox::EventEnd()
{
	//トリガーイベントなら、終了を内部の処理に任せる
	if (EventManager::Get()->CheckExestEvent(eventName_) != nullptr) {
		if (EventManager::Get()->CheckExestEvent(eventName_)->get()->isTrigerEvent)
		{
			return;
		}
	}

	if (!trigger)
	{
		return;
	}
	EventManager::Get()->End(eventName_);

	trigger = false;
}