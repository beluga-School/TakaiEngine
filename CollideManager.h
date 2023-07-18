#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"
#include "Box.h"
#include "Mob.h"
#include "Block.h"

//���X�g�̒��ɓ����I�u�W�F�N�g������΁A���̃I�u�W�F�N�g�����Ȃ��v�b�V���o�b�N
template <class T>
void UniqueObjectPushBack(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//�����v�f������������~�߂�
		if (*itr == col)
		{
			return;
		}
		//�񂵂��ꂽ�瓯���v�f���Ȃ�
	}
	//��������
	list.push_back(col);
}

//���X�g�̒��ɓ����I�u�W�F�N�g������΁A���̃I�u�W�F�N�g���폜����
template <class T>
void UniqueObjectErase(std::list<T>& list, const T& col)
{
	for (auto itr = list.begin(); itr != list.end(); itr++)
	{
		//�����v�f������������
		if (*itr == col)
		{
			//����
			list.erase(itr);
			return;
		}
	}
}

class CollideManager
{
public:
	std::list<Box*> allCols;
	void Update();

	static CollideManager* Get()
	{
		static CollideManager instance;
		return &instance;
	}

	//�蓮�œ����蔻���ǂ�ł��������Ƃ��p�ɁA�ʂŕ�����
	void CheckCollide(Box* check,Box* collide);

private:
	CollideManager(){};
	~CollideManager(){};

	//�����߂�����
	void Osimodosi(Mob& check, const Block& collide);

	//check��collide�̈ʒu�֌W�����āA�E�ɂ��邩���ɂ��邩�Ȃǂ�true/false��Ԃ�
	bool CheckDirections(const Cube& check, const Cube& collide, const CheckDirection& CD);
};