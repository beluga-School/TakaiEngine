#pragma once
#include "Collision.h"

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
	Cannon = 2,
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
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//�}�l�[�W���[�Ɏ��g��o�^
	void Register();

	//�Y�܂ꂽ���_�Ń}�l�[�W���[�ɓo�^
	Box()
	{
		Register();
	}
};