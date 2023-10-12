#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "UI.h"

class TutorialUIMove : public IEvent
{
public:
	TutorialUIMove() : IEvent()
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