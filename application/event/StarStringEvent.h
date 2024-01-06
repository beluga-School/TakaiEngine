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
	Vector2 pos;
	Vector2 startPos;
	Vector2 endPos;
	TEasing::easeTimer spawnTimer = 1.0f;
	TEasing::easeTimer invideTimer = 0.5f;
	TEasing::easeTimer backTimer = 1.0f;
	
};

