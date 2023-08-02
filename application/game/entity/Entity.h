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
	Player = 5,
	Enemy = 6,
	Star = 7,
	WarpBlock = 8,
	Coin = 9,
	Dokan = 10,
	NoDraw = 11,
};

//3d�`�悳���S�ẴI�u�W�F�N�g
class Entity : public Obj3d
{
public:
	bool IsDead() const {
		return isDead;
	}

	/// <summary>
	/// �w�肵���^�O�����邩�m�F����
	/// </summary>
	/// <param name="check">�m�F�������^�O</param>
	/// <returns>��������true �Ȃ��Ȃ�false</returns>
	bool CheckTag(TagTable check);

	/// <summary>
	/// �w�肵���^�O��t����(�d�����Ă����ꍇ�͕t���Ȃ�)
	/// </summary>
	/// <param name="check">�t�������^�O</param>
	/// <returns>�^�O�̃Z�b�g�ɐ�����true �d������������false</returns>
	bool SetTag(TagTable check);
	
	/// <summary>
	/// �w�肵���^�O���폜����
	/// </summary>
	/// <param name="check">�폜�������^�O</param>
	/// <returns>�^�O�̍폜�ɐ�����true �^�O��������Ȃ��ꍇfalse</returns>
	bool DeleteTag(TagTable check);

	void Register();

	//Entity�͕K���l�p�̓����蔻�������
	Box box;

	Entity()
	{
		SetTag(TagTable::Box);
	}

protected:
	bool isDead = false;

private:
	//��������Ƃ��Ɏg���^�O
	std::vector<TagTable> taglist;
};