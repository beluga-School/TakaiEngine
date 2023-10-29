#include "Clear1.h"
#include "StageChanger.h"
#include "GoalSystem.h"

void Clear1::Start()
{
	setFlag = false;
	timer.Start();
}

void Clear1::End()
{
}

void Clear1::Initialize()
{
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

bool Clear1::EndFlag()
{
	return timer.GetEnd();
}
