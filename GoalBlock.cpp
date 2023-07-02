#include "GoalBlock.h"
#include "Stage.h"

void GoalBlock::HitEffect()
{
	if (LevelLoader::Get()->GetData(trigerName) != nullptr)
	{
		//mGoalSyetem.mIsClear = true;
	}
}
