#include "Clear1.h"
#include "Stage.h"
#include "GoalSystem.h"
#include "Player.h"

void Clear1::Start()
{
	setFlag = false;
	timer.Start();

	ClearManage::Get()->isClear = false;
	ClearManage::Get()->eventNumber = -1;
}

void Clear1::Update()
{
	timer.Update();
	//ブロックを特定の位置に置く
	if (timer.GetTimeRate() >= 0.5f && setFlag == false)
	{
		for (auto& objectData : LevelLoader::Get()->GetData("clear1")->mObjects)
		{
			StageChanger::Get()->NormalObjectSet(objectData);
		};
		setFlag = true;
	}
}

void Clear1::Draw()
{
}

bool Clear1::End()
{
	return timer.GetEnd();
}
