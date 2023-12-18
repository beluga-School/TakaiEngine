#pragma once
#include "IEvent.h"

class MessageEvent : public IEvent
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

	std::string messageTexHandle = "";

private:
	TEasing::easeTimer endTimer = 1.0f;
};

