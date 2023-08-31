#include "Mob.h"
#include "TimeManager.h"

void Mob::CollsionUpdate()
{
	//�����蔻��̊ۂ�
	CalcNearestHitLists();

	if (moveBlockHit)
	{
		position += moveBlockPosition;
	}
	
	//�{���Z
	//�����Ȃ��t���O����������Z���Ȃ�
	if (mNoMove == false)
	{
		position += moveValue;
	}

	//����X������\��
	UpdateX();

	UpdateY();

	moveBlockPosition = { 0,0,0 };

	moveBlockHit = false;
}

void Mob::UpdateY()
{
	JumpUpdate();
}

void Mob::UpdateX()
{
	//�E
	if (position.x <= hitLeftMin)
	{
		position.x = hitLeftMin;
	}
	//��
	if (position.x >= hitRightMin)
	{
		position.x = hitRightMin;
	}
	//����
	if (position.z >= hitCenterMin)
	{
		position.z = hitCenterMin;
	}
	//����
	if (position.z <= hitBackMin)
	{
		position.z = hitBackMin;
	}
}

void Mob::CalcNearestHitLists()
{
	//�R���W�������s��Ȃ��t���O�������Ă���Ȃ�X�L�b�v
	if (mNoCollision)return;

	//�������̔���
	float preDownY = -114514.f;
	float maxDownY = 0;

	hitFeetMax = preDownY;

	for (auto& hit : hitListDown)
	{
		maxDownY = hit.position.y;
		//�����l�łȂ��A�O�̒l��荂���ʒu�ɂ���Ȃ�
		if (maxDownY >= preDownY)
		{
			//��ԍ������W���Z�o
			//���������������āA�u���b�N�̐؂�ڂɈ���������Ȃ��悤��
			feet = hit.position.y + hit.scale.y / 2;
			//Q,�����̃X�P�[��/2����Ȃ���
			//A,�����̃X�P�[��/2�́A����ǂ̑����݂����Ȃ��̂Ȃ̂ŁA�Œ�̑�������������΂����Ǝv���܂�
			hitFeetMax = feet + scale.y / 2 + 0.01f;
		}
		preDownY = hit.position.y;
	}

	//������̔���
	float preUpY = 114514.f;
	float maxUpY = 0;

	hitCeilingMax = preUpY;

	for (auto& hit : hitListUp)
	{
		maxUpY = hit.position.y;
		if (maxUpY <= preUpY)
		{
			hitCeilingMax = hit.position.y - hit.scale.y / 2 - scale.y / 2;
		}
		preUpY = hit.position.y;
	}

	//X������
	
	//�����̔���
	float preLeft = -114514.f;
	float maxLeft = 0;

	hitLeftMin = preLeft;

	for (auto& hit : hitListLeft)
	{
		maxLeft = hit.position.x;
		if (maxLeft >= preLeft)
		{
			hitLeftMin = hit.position.x + hit.scale.x / 2 + scale.x / 2;
		}
		preLeft = hit.position.x;
	}

	//�����̔���
	float preRight = 114514.f;
	float maxRight = 0;

	hitRightMin = preRight;

	for (auto& hit : hitListRight)
	{
		maxRight = hit.position.x;
		if (maxRight <= preRight)
		{
			hitRightMin = hit.position.x - hit.scale.x / 2 - scale.x / 2;
		}
		preRight = hit.position.x;
	}

	//Z������
	float preCenter = 114514.f;
	float maxCenter = 0;

	hitCenterMin = preCenter;

	for (auto& hit : hitListCenter)
	{
		maxCenter = hit.position.z;
		if (maxCenter <= preCenter)
		{
			hitCenterMin = hit.position.z - hit.scale.z / 2 - scale.z / 2;
		}
		preCenter = hit.position.z;
	}

	float preBack = -114514.f;
	float maxBack = 0;

	hitBackMin = preBack;

	for (auto& hit : hitListBack)
	{
		maxBack = hit.position.z;
		if (maxBack >= preBack)
		{
			hitBackMin = hit.position.z + hit.scale.z / 2 + scale.z / 2;
		}
		preBack = hit.position.z;
	}
}

void Mob::JumpUpdate()
{
	jumpManageTimer.Update();
	stayManageTimer.Update();

	switch (jumpState)
	{
	case Mob::JumpState::None:

		for (auto& hit : hitListDown)
		{
			if (hit.parentEntity->CheckTag(TagTable::MoveBlock))
			{
				if (moveBlockPosition.y <= 0.001)
				{
					position.y = hitFeetMax - 0.01f;
				}
			}
		}

		if (position.y > hitFeetMax)
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

		if (hitCeilingMax <= position.y)
		{
			jumpState = JumpState::Down;
			jumpManageTimer.Reset();
		}

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
			jumpState = JumpState::Down;
			stayManageTimer.Reset();
		}

		break;
	case Mob::JumpState::Down:
		//hitList�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g��莩�g�̍��W������������
		if (position.y > hitFeetMax)
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