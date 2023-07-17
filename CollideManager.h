#pragma once
#include "Collision.h"
#include <list>
#include "Obj.h"

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

//�u���b�N�̕������m�F���邽�߂�enum
enum class CheckDirection
{
	CD_UP,
	CD_DOWN,
	CD_CENTER,
	CD_BACK,
	CD_LEFT,
	CD_RIGHT,
};

//�����蔻������Ȃ�A���̓����蔻��I�u�W�F�N�g�S�ĂƔ�����Ƃ�
//->������惊�X�g�ɓ����
//�����͐������Ȃ̂ŁA�S���̔���Ǝ�����Ă����^�O(string)�Ŕ��ʂł���悤�ɂ���
//�����͒n�ʂƂ��Ȃ̂ŁA�������Ƃ�����������Ȃ�����Ă����^�O�Ŕ���

enum class TagTable
{
	None = -1,
	Mob = 0,
	Block = 1,
};

//�l�p�Ŕ�����Ƃ�I�u�W�F�N�g�̋��ʍ��������o�����N���X
class Box : public Obj3d
{
public:
	Cube cubecol;

	//��������Ƃ��Ɏg���^�O
	TagTable tag = TagTable::None;

	//���g�̓����蔻����쐬(�X�V)
	void CreateCol();
	void CreateCol(const Vector3& pos,const Vector3 &scale);

	//�}�l�[�W���[�Ɏ��g��o�^
	void Register();
	
	//�Y�܂ꂽ���_�Ń}�l�[�W���[�ɓo�^
	Box()
	{
		Register();
	}
};

class Block : public Box
{
public:
	Block() {
		tag = TagTable::Block;
	};
};

class Mob : public Box
{
public:
	Vector3 moveValue = { 0,0,0 };

	std::list<Cube> hitListY;

	Mob() {
		tag = TagTable::Mob;
	};
};

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