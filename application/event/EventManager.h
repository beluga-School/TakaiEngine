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

	//線イベント(上下線が出て、それの実行中は他の上下線が出るイベントが実行されない)を検索して返す
	std::unique_ptr<IEvent>* GetLineEvent();

	//ステージにそのイベントが存在するならそのイベントを取得
	std::unique_ptr<IEvent>* CheckExestEvent(const std::string& eventName);

	//現在実行中イベントにそのイベントがあればそのイベントを取得
	std::unique_ptr<IEvent>* GetRunEvent(const std::string& eventName);

	//現在実行中イベントにそのイベントがあればそのイベントを削除
	void DeleteRunEvent(const std::string& eventName);

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

	std::list<std::unique_ptr<IEvent>> allEvents;
	
	std::list<std::unique_ptr<IEvent>*> runEvents;

	//上下の線が出現/消滅する時間
	TEasing::easeTimer startTimer = 0.5f;
	TEasing::easeTimer endTimer = 0.5f;

	Vector2 uppos{};
	Vector2 downpos{};
};

