#pragma once
#include "IEvent.h"
class BossHikiCamera :
    public IEvent
{
public:
	BossHikiCamera() : IEvent()
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

