#include "GoalCamChange.h"
#include "GoalSystem.h"
#include "Stage.h"

void GoalCamChange::Start()
{
	timer.Reset();
}

void GoalCamChange::Update()
{
	//アップデートに入ってからタイマー開始して欲しいのでこの形に
	if (timer.GetStarted() == false)
	{
		timer.Start();
	}
	timer.Update();
}

void GoalCamChange::Draw()
{
}

bool GoalCamChange::End()
{
	return timer.GetEnd();
}
