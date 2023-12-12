#pragma once
#include "IEvent.h"
#include "TEasing.h"
#include "Enemy.h"

class EnemyPopEvent : public IEvent
{
public:
	EnemyPopEvent() : IEvent()
	{
		isTrigerEvent = true;
	}
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void Start()override;
	void End()override;

	bool EndFlag()override;

private:
	TEasing::easeTimer endTimer = 2.0f;

	std::list<Enemy*> enemys;
};

