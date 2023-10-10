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
	static void LoadResource();

	//イベント開始
	bool Start(const std::string& startEventName);

	//イベントを強制終了(nowEventを即時nullptrにする)
	//イベント名が指定されている場合、現在実行中のイベント名と一致した場合のみ終了処理を行う
	void ForceEnd(const std::string endEventName = "");

	//イベントの上下黒線をなくして、end状態に移行
	void End(const std::string endEventName = "");

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

	template <class Event> void Register(const std::string& eventName)
	{
		allEvents.emplace_back();
		allEvents.back() = std::make_unique<Event>();
		allEvents.back()->eventName = eventName;
	}

private:
	EventManager(){};
	~EventManager(){};

	enum class State
	{
		None,
		Start,
		RunEvent,
		End,
	};
	
	std::list<std::unique_ptr<IEvent>> allEvents;
	std::unique_ptr<IEvent>* nowEvent = nullptr;

	//上下の線が出現/消滅する時間
	TEasing::easeTimer startTimer = 0.5f;
	TEasing::easeTimer endTimer = 0.5f;

	Vector2 uppos{};
	Vector2 downpos{};

	State state = State::None;
};

