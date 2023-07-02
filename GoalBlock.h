#pragma once
#include "EventBlock.h"
#include "GoalSystem.h"

class GoalBlock : public EventBlock
{
public:
	void HitEffect()override;

	//GoalSystem mGoalSyetem;
};

