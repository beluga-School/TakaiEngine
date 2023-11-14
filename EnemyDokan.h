#pragma once
#include "EventBlock.h"
#include "TEasing.h"

class EnemyDokan : public EventBlock
{
public:
	EnemyDokan() :EventBlock()
	{

	};

	static void LoadResource();

	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

private:
	TEasing::easeTimer popCool = 5.0f;
};

