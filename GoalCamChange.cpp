#include "GoalCamChange.h"
#include "GoalSystem.h"
#include "Stage.h"

void GoalCamChange::Start()
{
	timer.Reset();
}

void GoalCamChange::Update()
{
	//�A�b�v�f�[�g�ɓ����Ă���^�C�}�[�J�n���ė~�����̂ł��̌`��
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
