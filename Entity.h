#pragma once
#include "Obj.h"
#include "Box.h"
#include <vector>

//�����蔻������Ȃ�A���̓����蔻��I�u�W�F�N�g�S�ĂƔ�����Ƃ�
//->������惊�X�g�ɓ����
//�����͐������Ȃ̂ŁA�S���̔���Ǝ�����Ă����^�O(string)�Ŕ��ʂł���悤�ɂ���
//�����͒n�ʂƂ��Ȃ̂ŁA�������Ƃ�����������Ȃ�����Ă����^�O�Ŕ���
enum class TagTable
{
	None = -1,
	Mob = 0,
	Block = 1,
	Cannon = 2,
	Box = 3,
	Collsion = 4,
};

//3d�`�悳���S�ẴI�u�W�F�N�g
class Entity : public Obj3d
{
public:
	//��������Ƃ��Ɏg���^�O
	std::vector<TagTable> taglist;

	bool IsDead() {
		return isDead;
	}

	bool CheckTag(TagTable check);

	void Register();

	//Entity�͕K���l�p�̓����蔻�������
	Box box;

	Entity()
	{
		//Register();
		taglist.push_back(TagTable::Box);
	}

protected:
	bool isDead = false;
};