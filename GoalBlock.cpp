#include "GoalBlock.h"
#include "Stage.h"

void GoalBlock::HitEffect()
{
	Stage::Get()->goalSystem.AnimetionStart();
}
