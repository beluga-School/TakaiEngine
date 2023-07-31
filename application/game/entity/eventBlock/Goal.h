#pragma once
#include "Obj.h"
#include "GoalBlock.h"
#include "EventBlock.h"
#include "ImguiManager.h"

class Goal : public EventBlock
{
public:
	GoalBlock goalBlock;

	void Initialize()override;
	void Update()override;
	void Draw()override;

	void HitEffect()override;
private:

	//�����ʒu��z�u���Ȃ����t���O
	bool mInitFragPos = false;

};

