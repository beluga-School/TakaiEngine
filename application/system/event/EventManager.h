#pragma once
#include "Vector2.h"
#include "TEasing.h"
#include <string>
#include <list>
#include "IEvent.h"
#include <memory>

class EventManager
{
public:
	//イベント開始
	bool Start(const std::string& startEventName);

	//イベントを強制終了(nowEventを即時nullptrにする)
	void ForceEnd();

	//現在実行中のイベントを取得
	std::unique_ptr<IEvent>* GetNowEvent();

	//イベント名からイベントを取得
	std::unique_ptr<IEvent>* GetEvent(const std::string& eventName);

	static EventManager* Get()
	{
		static EventManager instance;
		return &instance;
	}

	void Initialize();

	void Update();
	
	//イベント中用の線を表示
	void Draw();

	//登録されたイベントをリセット
	void Clear();

	template <class Event> void Register(const EventCamData& camdata,const std::string& eventName)
	{
		allEvents.emplace_back();
		allEvents.back() = std::make_unique<Event>();
		allEvents.back()->eventName = eventName;
		allEvents.back()->eventCamera.eventCamData = camdata;
		allEvents.back()->eventCamera.Initialize();
	}

private:
	EventManager(){};
	~EventManager(){};

	//イベントの上下黒線をなくして、end状態に移行
	void End();

	enum class State
	{
		None,
		Start,
		RunEvent,
		End,
	};
	
	std::list<std::unique_ptr<IEvent>> allEvents;
	std::unique_ptr<IEvent>* nowEvent = nullptr;

	TEasing::easeTimer startTimer = 0.5f;
	TEasing::easeTimer endTimer = 0.5f;

	Vector2 uppos{};
	Vector2 downpos{};

	State state = State::None;
};

