#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "UI.h"

class TutorialUI_1 : public IEvent
{
public:
	TutorialUI_1() : IEvent()
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

