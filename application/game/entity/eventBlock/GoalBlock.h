#pragma once
#include "Entity.h"
#include "GoalSystem.h"
#include "TEasing.h"

/*! GoalBlock
	触れるとゴール処理が入るクラス
*/
class GoalBlock : public Entity
{
public:
	void Initialize()override;

	void Update()override;

	void Draw()override;

	//ヒット時の処理
	void HitEffect();

private:
	TEasing::easeTimer goalAnimetionTimer = 2.0f;
};

