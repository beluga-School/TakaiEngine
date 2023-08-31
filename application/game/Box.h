#pragma once
#include "Collision.h"
#include "Tag.h"
#include <list>

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

class Entity;

//�����蔻��p�ɁAID���ʂ��ł���悤�ɂ���Cube
struct IDdCube : public Cube
{
	IDdCube()
	{
		//ID��Count�𑝂₷
		IDdCube::sMasterIDCount++;

		//Entity���Y�܂ꂽ��A�����݂̂ɑΉ�����ID������U��
		//�X�e�[�W�ړ��̍ۂ�ID������������\��Ȃ̂ŁAID���d������\��������
		masterID = IDdCube::sMasterIDCount;
	}

	//���g��ID���擾����֐�
	uint32_t GetID()const;

	/// <summary>
	/// �t�^ID��0�ɖ߂��֐�
	/// �S�I�u�W�F�N�g�����̍ۂ݂̂Ɏg�p���A����ȊO�ł͎g�p���Ȃ�����
	/// </summary>
	static void ResetID();

	Entity* parentEntity = nullptr;

private:
	//�S�ẴI�u�W�F�N�g���A���g�����̃}�C�i���o�[������
	uint32_t masterID = 0;

	//���ɕt�^����ID���Ǘ�����i���o�[
	static uint32_t sMasterIDCount;
};

//�l�p�Ŕ�����Ƃ�I�u�W�F�N�g�̋��ʍ��������o�����N���X
class Box : public Obj3d
{
public:
	IDdCube cubecol;

	//���g�̓����蔻����쐬(�X�V)
	void CreateCol(const Vector3& pos, const Vector3& scale);

	//���g�̕`��I�u�W�F�N�g���X�V
	void ColDrawerUpdate(const Vector3& pos, const Vector3& scale);

	Box()
	{
		
	}
};