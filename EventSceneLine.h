#pragma once
#include "Vector2.h"
#include "TEasing.h"

class EventSceneLine
{
public:
	Vector2 uppos{};
	Vector2 downpos{};
	
	void Start();
	void End();

	void Initialize();
	void Update();
	void Draw();

private:
	TEasing::easeTimer startTimer = 0.5f;
	TEasing::easeTimer endTimer = 0.5f;

	enum class State
	{
		None,
		Start,
		End,
	}state = State::None;
};

