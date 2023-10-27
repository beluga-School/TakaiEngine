#pragma once
#include "EventBlock.h"
#include "GoalSystem.h"
#include "TEasing.h"

/*! GoalBlock
	触れるとゴール処理が入るクラス
*/
class GoalBlock : public EventBlock
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	void HitEffect()override;

private:
	TEasing::easeTimer goalAnimetionTimer = 2.0f;
};

