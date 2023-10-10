#include "EventTriggerBox.h"
#include <Collision.h>
#include <EventManager.h>

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
