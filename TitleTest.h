#pragma once
#include <IEvent.h>
#include "TEasing.h"

class TitleTest : public IEvent
{
public:
	TitleTest() : IEvent()
	{
		isUseEventLine = false;
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;
};

