#pragma once
#include "Vector2.h"
#include "TEasing.h"

class EventManager
{
public:
	//イベント開始
	static void Start();
	//イベント終了
	static void End();

	//現在がイベント中かどうかを取得
	static bool IsNowEvent();

	static void Initialize();

	static void Update();
	
	//イベント中用の線を表示
	static void Draw();

private:
	EventManager(){};
	~EventManager(){};

	enum class State
	{
		None,
		Start,
		End,
	};
	
	static TEasing::easeTimer startTimer;
	static TEasing::easeTimer endTimer;

	static Vector2 uppos;
	static Vector2 downpos;

	static State state;
};

