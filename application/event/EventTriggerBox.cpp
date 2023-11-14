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
}

void EventTriggerBox::Draw()
{
	Obj3d::Draw();
}

void EventTriggerBox::EventStart()
{
	if (trigger)
	{
		return;
	}
	EventManager::Get()->Start(eventName_);
	trigger = true;
}

void EventTriggerBox::EventEnd()
{
	if (!trigger)
	{
		return;
	}
	EventManager::Get()->End(eventName_);
	trigger = false;
}
