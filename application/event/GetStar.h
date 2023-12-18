#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "Star.h"

class GetStar : public IEvent
{
public:
	GetStar() : IEvent() {
		isUseEventLine = false;
		isTrigerEvent = true;
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

private:
	//Star* star = nullptr;
};

