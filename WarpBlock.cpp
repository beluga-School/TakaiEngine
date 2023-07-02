#include "WarpBlock.h"
#include "Stage.h"

void WarpBlock::HitEffect()
{
	if (LevelLoader::Get()->GetData(trigerName) != nullptr)
	{
		Stage::Get()->ChangeLevel(*LevelLoader::Get()->GetData(trigerName));
	}
}
