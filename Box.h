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

//�l�p�Ŕ�����Ƃ�I�u�W�F�N�g�̋��ʍ��������o�����N���X
class Box : public Obj3d
{
public:
	Cube cubecol;

	//���g�̓����蔻����쐬(�X�V)
	void CreateCol();
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//�Y�܂ꂽ���_�Ń}�l�[�W���[�ɓo�^
	Box()
	{
		//Register();
	}
};