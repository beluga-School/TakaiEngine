#pragma once
#include "Obj.h"
#include "GoalBlock.h"
#include "Entity.h"
#include "ImguiManager.h"

/*! Goal
	ゴールオブジェクト
*/
class Goal : public Entity
{
public:
	GoalBlock goalBlock;

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect();

private:

	//初期位置を配置しなおすフラグ
	bool mInitFragPos = false;

};

