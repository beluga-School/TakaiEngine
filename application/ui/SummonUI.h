#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "UI.h"

class SummonUI : public IEvent
{
public:
	SummonUI() : IEvent()
	{
		isUseEventLine = false;
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

private:

};

