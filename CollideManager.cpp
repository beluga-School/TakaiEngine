#include "CollideManager.h"
#include "TimeManager.h"
#include "Mob.h"
#include "Block.h"
#include "Cannon.h"


#include "Player.h"
#include "ImguiManager.h"

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

bool CheckIsDead(Entity* box)
{
	return box->IsDead();
}

void CollideManager::Update()
{
	//����ł���Ȃ�폜����
	for (auto itr = allCols.begin(); itr != allCols.end();)
	{
		if (CheckIsDead(*itr))
		{
			itr = allCols.erase(itr);
		}
		else
		{
			itr++;
		}
	}
	for (Entity*check : allCols)
	{
		for (Entity* collide : allCols)
		{
			CheckCollide(check, collide);
		}
	}
}

void CollideManager::CheckCollide(Entity* check, Entity* collide)
{
	//�����Ȃ画�肵�Ȃ���
	if (check == collide)
	{
		return;
	}

	//���肷�鑤�����u�̎�
	if (check->CheckTag(TagTable::Mob))
	{
		if (check == nullptr)
		{
			return;
		}

		//check��mob�ł��邱�Ƃ͊m�肵�Ă���̂ŁAmob�^�ɕϊ����ăf�[�^�������Ă���
		Mob* mob = static_cast<Mob*>(check);

		//����鑤���u���b�N�Ȃ�
		if (collide->CheckTag(TagTable::Block))
		{
			//collide��Block�ł��邱�Ƃ͊m�肵�Ă���̂ŁABlock�^�ɕϊ����ăf�[�^�������Ă���
			Block* block = static_cast<Block*>(collide);

			//�����߂��������s��
			Osimodosi(*mob, *block);
		}
		if (collide->CheckTag(TagTable::Cannon))
		{
			//collide��Cannon�ł��邱�Ƃ͊m�肵�Ă���̂ŁACannon�^�ɕϊ����ăf�[�^�������Ă���
			Cannon* cannon = static_cast<Cannon*>(collide);
			if (Collsions::CubeCollision(mob->box.cubecol, cannon->box.cubecol))
			{
				cannon->OnCollide(*mob);
			}
		}
	}
}

void CollideManager::Osimodosi(Mob& check, const Block& collide)
{
	//���̃I�u�W�F�N�g���
	//��ɂ��邩
	bool up = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_UP);
	//���ɂ��邩
	bool down = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_DOWN);
	//�O�ɂ��邩
	bool center = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_CENTER);
	//���ɂ��邩
	bool back = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_BACK);
	//���ɂ��邩
	bool left = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_LEFT);
	//�E�ɂ��邩
	bool right = CheckDirections(check.box.cubecol, collide.box.cubecol, CheckDirection::CD_RIGHT);

	//��ʂ̓����蔻��
	//������Ō����Ƃ��ɉ��ɂ���I�u�W�F�N�g�������
	Cube rayCube;
	rayCube.position = check.position;
	rayCube.scale = check.scale;
	//�X�P�[�����߂�����������΂�
	rayCube.scale.y = 100;

	//���������Ȃ�
	//������=���@��
	bool cubeCol = Collsions::CubeCollision(rayCube, collide.box.cubecol);

	bool upCol = up && cubeCol;

	//�S��0�Ȃ疄�܂��Ă���
	bool isBuried = !up && !down && !right && !left && !back && !center;

	//��ʂ��������Ă��邩�I�u�W�F�N�g�����܂��Ă���Ȃ�
	if (upCol || isBuried)
	{
		//���X�g�ɓ����
		UniqueObjectPushBack(check.hitListY, collide.box.cubecol);
	}
	else
	{
		//�����łȂ��Ȃ�O��
		UniqueObjectErase(check.hitListY, collide.box.cubecol);
	}
	

	//���ʂ̓����蔻��
	if (down)
	{

	}

	//���E�̓����蔻��
	if (up == false)
	{
		//���E���ʂ̓����蔻�胊�X�g����āA������@�Ŏ�ꂻ���H
		if (Collsions::CubeCollision(check.box.cubecol, collide.box.cubecol))
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
