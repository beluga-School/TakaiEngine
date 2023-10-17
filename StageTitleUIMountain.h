#pragma once
#include "IEvent.h"

class StageTitleUIMountain : public IEvent
{
public:
	StageTitleUIMountain() : IEvent()
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

