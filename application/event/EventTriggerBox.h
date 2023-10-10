#pragma once
#include <Entity.h>

class EventTriggerBox : public Entity
{
public:
	EventTriggerBox() : Entity()
	{
		SetTag(TagTable::EventTriggerBox);
	}

	void EventStart();

	void EventEnd();

	std::string eventName_ = "";

private:
	bool trigger = false;
};

