#pragma once
#include "TEasing.h"

class CountDownTimer
{
public:
	static void LoadResource();

	void SetTime(float maxTime);
	void Start();

	void Update();
	void Draw();

	bool mActive = false;

	TEasing::easeTimer timer = 1.0f;
};

