#pragma once
#include "IEvent.h"
#include "TEasing.h"

class GoalCamChange :public IEvent
{
public:
	void Start()override;
	void Update()override;
	void Draw()override;

	//一応時間で終了するが、強制終了した方が安全
	bool End()override;

private:
	TEasing::easeTimer timer = 2.0f;
};

