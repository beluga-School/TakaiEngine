#pragma once
#include "IEvent.h"
#include "TEasing.h"

class Clear1 : public IEvent
{
public:
	void Start()override;
	void Update()override;
	void Draw()override;

	bool End()override;

	TEasing::easeTimer timer = 2.0f;
	bool setFlag = false;
};

