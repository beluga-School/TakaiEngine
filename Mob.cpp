#include "Mob.h"
#include "TimeManager.h"

void Mob::UpdateY()
{
	JumpUpdate();

	GroundCol();
}

void Mob::GroundCol()
{
	//������̔���
	float preY = -114514.f;
	float maxY = 0;

	hitCubeMaxY = preY;

	for (auto& hit : hitListY)
	{
		maxY = hit.position.y;
		//�����l�łȂ��A�O�̒l��荂���ʒu�ɂ���Ȃ�
		if (maxY >= preY)
		{
			//��ԍ������W���Z�o
			//���������������āA�u���b�N�̐؂�ڂɈ���������Ȃ��悤��
			feet = hit.position.y + hit.scale.y / 2;
			hitCubeMaxY = feet + scale.y / 2 + 0.01f;
		}
		preY = hit.position.y;
	}

	//�������Z�o�����ő�l�ɍ��킹��
	if (jumpState == JumpState::None)
	{
		if (position.y > hitCubeMaxY)
		{
			jumpState = JumpState::Down;
		}
		else
		{
			position.y = hitCubeMaxY;
			gravity = 0;
		}
	}
}

void Mob::JumpUpdate()
{
	jumpManageTimer.Update();
	stayManageTimer.Update();

	switch (jumpState)
	{
	case Mob::JumpState::None:

		if (position.y > hitCubeMaxY)
		{
			jumpState = JumpState::Down;
		}
		else
		{
			//�n�ʂɗ����Ă����Ԃɂ���
			gravity = 0;
		}

		break;
	case Mob::JumpState::Up:
		//�C�[�W���O�ŏ㏸
		position.y = TEasing::OutQuad(upJumpS, upJumpE, jumpManageTimer.GetTimeRate());

		//���Ԃ��I�������X�e�[�g�����̏�ԂɑJ��
		if (jumpManageTimer.GetEnd())
		{
			jumpState = JumpState::Staying;
			jumpManageTimer.Reset();
			stayManageTimer.Start();
		}

		break;
	case Mob::JumpState::Staying:
		if (stayManageTimer.GetEnd())
		{
			jumpState = JumpState::None;
			stayManageTimer.Reset();
		}

		break;
	case Mob::JumpState::Down:
		//hitList�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g��莩�g�̍��W������������
		if (position.y > hitCubeMaxY)
		{
			//�d�͗���������
			if (!noGravity)
			{
				gravity += gravityAdd;
				position.y -= gravity * TimeManager::deltaTime;
			}
		}
		//hitList�I�u�W�F�N�g�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g�Ɏ��g���������Ă���Ȃ�
		else
		{
			jumpState = JumpState::None;
		}

		break;
	}
}