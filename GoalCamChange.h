#pragma once
#include "IEvent.h"
#include "TEasing.h"

class GoalCamChange :public IEvent
{
public:
	void Start()override;
	void Update()override;
	void Draw()override;

	//�ꉞ���ԂŏI�����邪�A�����I�������������S
	bool End()override;

private:
	TEasing::easeTimer timer = 2.0f;
};

