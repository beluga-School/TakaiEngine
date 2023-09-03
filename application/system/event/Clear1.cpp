#include "Clear1.h"
#include "Stage.h"
#include "GoalSystem.h"

void Clear1::Start()
{
	setFlag = false;
	timer.Start();
}

void Clear1::Update()
{
	timer.Update();
	//�u���b�N�����̈ʒu�ɒu��
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
