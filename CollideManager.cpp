#include "CollideManager.h"
#include "TimeManager.h"

bool CollideManager::CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD)
{
	switch (CD)
	{
	case CheckDirection::CD_UP:
		return check.position.y > collide.position.y + collide.scale.y * 0.5f;
		break;
	case CheckDirection::CD_DOWN:
		return check.position.y < collide.position.y - collide.scale.y * 0.5f;
		break;
	case CheckDirection::CD_CENTER:
		return check.position.z > collide.position.z + collide.scale.z * 0.5f;;
		break;
	case CheckDirection::CD_BACK:
		return check.position.z < collide.position.z - collide.scale.z * 0.5f;
		break;
	case CheckDirection::CD_LEFT:
		return check.position.x > collide.position.x + collide.scale.x * 0.5f;
		break;
	case CheckDirection::CD_RIGHT:
		return check.position.x < collide.position.x - collide.scale.x * 0.5f;
		break;
	}
	return false;
}

void Box::CreateCol()
{
	cubecol.position = position;
	cubecol.scale = scale;
}

void Box::CreateCol(const Vector3& pos_, const Vector3& scale_)
{
	cubecol.position = pos_;
	cubecol.scale = scale_;
}

void Box::Register()
{
	CollideManager::Get()->allCols.push_back(this);
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
			gravity += gravityAdd;
			position.y -= gravity * TimeManager::deltaTime;
		}
		//hitList�I�u�W�F�N�g�̒��ŁA�ł������ʒu�ɂ���I�u�W�F�N�g�Ɏ��g���������Ă���Ȃ�
		else
		{
			jumpState = JumpState::None;
		}

		break;
	}
}

void CollideManager::Update()
{
	//�������X�g���̑��̃I�u�W�F�N�g�S�Ă̐����������
	for (Box*check : allCols)
	{
		for (Box* collide : allCols)
		{
			CheckCollide(check, collide);
		}
	}
}

void CollideManager::CheckCollide(Box* check, Box* collide)
{
	//�����Ȃ画�肵�Ȃ���
	if (check == collide)
	{
		return;
	}

	//���肷�鑤�����u�̎�
	if (check->tag == TagTable::Mob)
	{
		if (check == nullptr)
		{
			return;
		}
		//����鑤���u���b�N�Ȃ�
		if (collide->tag == TagTable::Block)
		{
			//check��mob�ł��邱�Ƃ͊m�肵�Ă���̂ŁAmob�^�ɕϊ����ăf�[�^�������Ă���
			Mob* mob = static_cast<Mob*>(check);

			//collide��Block�ł��邱�Ƃ͊m�肵�Ă���̂ŁABlock�^�ɕϊ����ăf�[�^�������Ă���
			Block* block = static_cast<Block*>(collide);

			//�����߂��������s��
			Osimodosi(*mob, *block);
		}
	}
}

void CollideManager::Osimodosi(Mob& check, const Block& collide)
{
	//���̃I�u�W�F�N�g���
	//��ɂ��邩
	bool up = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_UP);
	//���ɂ��邩
	bool down = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_DOWN);
	//�O�ɂ��邩
	bool center = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_CENTER);
	//���ɂ��邩
	bool back = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_BACK);
	//���ɂ��邩
	bool left = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_LEFT);
	//�E�ɂ��邩
	bool right = CheckDirections(check.cubecol, collide.cubecol, CheckDirection::CD_RIGHT);

	//��ʂ̓����蔻��
	if (up)
	{
		//������Ō����Ƃ��ɉ��ɂ���I�u�W�F�N�g�������
		Cube rayCube;
		rayCube.position = check.position;
		rayCube.scale = check.scale;
		//�X�P�[�����߂�����������΂�
		rayCube.scale.y = 100;

		//���������Ȃ�
		//������=���@��
		bool cubeCol = Collsions::CubeCollision(rayCube, collide.cubecol);

		if (cubeCol)
		{
			//���X�g�ɓ����

			//���̃��X�g���������������̂ł͂Ȃ��A
			//�����蔻����Ƃ�A�����v�f�������Ă��Ȃ��Ȃ�����
			//�����蔻�肪�O�ꂽ�Ƃ��ɁA���̗v�f������
			UniqueObjectPushBack(check.hitListY, collide.cubecol);
		}
		else
		{
			UniqueObjectErase(check.hitListY, collide.cubecol);
		}
	}
	

	//���ʂ̓����蔻��
	if (down)
	{

	}

	//���E�̓����蔻��
	if (up == false)
	{
		//���E���ʂ̓����蔻�胊�X�g����āA������@�Ŏ�ꂻ���H
		if (Collsions::CubeCollision(check.cubecol, collide.cubecol))
		{
			//�������������傫�����āA�����蔻���������I�u�W�F�N�g�Ɠ������Ă���Ȃ�
			if (right)
			{
				check.moveValue.x = 0;
			}
			if (left)
			{
				check.moveValue.x = 0;
			}
			if (back)
			{
				check.moveValue.z = 0;
			}
			if (center)
			{
				check.moveValue.z = 0;
			}
		}
	}
}
