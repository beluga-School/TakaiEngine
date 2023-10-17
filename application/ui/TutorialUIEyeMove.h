#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "UI.h"

class TutorialUIEyeMove : public IEvent
{
public:
	TutorialUIEyeMove() : IEvent()
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

