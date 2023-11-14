#pragma once
#include <Entity.h>

/*! EventTriggerBox
	触れるとイベントを発生させるクラス
*/
class EventTriggerBox : public Entity
{
public:
	EventTriggerBox() : Entity()
	{
		SetTag(TagTable::EventTriggerBox);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void EventStart();

	void EventEnd();

	//std::string eventName_ = "";

private:
	bool trigger = false;
};

