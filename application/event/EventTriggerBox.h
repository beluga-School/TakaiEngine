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

	//イベント開始処理
	void EventStart();

	//イベント終了処理
	void EventEnd();

private:
	bool trigger = false;
};

