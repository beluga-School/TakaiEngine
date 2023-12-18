#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "RedCoin.h"

class RedCoinEvent : public IEvent
{
public:
	RedCoinEvent() : IEvent() {
		isUseEventLine = false;
		isTrigerEvent = true;
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

};

