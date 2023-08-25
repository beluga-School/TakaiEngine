#pragma once
#include "Vector2.h"
#include "TEasing.h"
#include <string>
#include <list>

class EventManager
{
public:
	//イベント開始
	static bool Start(const std::string& startEventName);
	//イベント終了
	static void End();

	//現在のイベントを取得
	std::string GetNowEvent();

	static EventManager* Get()
	{
		static EventManager instance;
		return &instance;
	}

	static void Initialize();

	static void Update();
	
	//イベント中用の線を表示
	static void Draw();

	//イベントを登録
	static void Register(const std::string& startEventName);

	static void Clear();

private:
	EventManager(){};
	~EventManager(){};

	enum class State
	{
		None,
		Start,
		End,
	};
	
	std::list<std::string> allEvents;

	//現在発生させているイベント
	std::string nowEvent;

	static TEasing::easeTimer startTimer;
	static TEasing::easeTimer endTimer;

	static Vector2 uppos;
	static Vector2 downpos;

	static State state;
};

