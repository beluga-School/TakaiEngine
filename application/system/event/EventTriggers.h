#pragma once
#include <string>

struct EventTrigger
{
	//イベントトリガーの名前
	std::string triggerName = "";

	//条件が満たされていたらtrueになる
};

//イベントを実行するための条件を集めてフラグ化するクラス
class EventTriggers
{
public:
	static EventTriggers* Get()
	{
		static EventTriggers instance;
		return &instance;
	}

private:
	EventTriggers() {};
	~EventTriggers() {};
};

