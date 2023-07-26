#pragma once
#include "Obj.h"
#include "Collision.h"
#include "Mob.h"

//�s���e�[�u�� ���̒�����K�v�Ȃ��̂��������o���Ďg���݌v
enum class ActTable
{
	None,		//�������Ă��Ȃ����
	Encount,	//�v���C���[�𔭌��������
	Tracking,
	Staying,
	Dead,		//���S���
};

class Enemy : public Mob
{
public:
	Enemy() : Mob()
	{
		taglist.push_back(TagTable::Enemy);
	}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;

	/// <summary>
	/// ���݂̃e�[�u���������ƈ�v���Ă���Ȃ�true��Ԃ�
	/// </summary>
	/// <param name="table">ActTable����m�F�������X�e�[�g������</param>
	/// <returns></returns>
	bool GetNowAct(ActTable table) {
		return mActTable == table;
	};

	Sphere sphereCol{};

	int32_t GetHitDamage();

protected:
	/// <summary>
	/// �w�肵���^�[�Q�b�g�ւ̃x�N�g����Ԃ�
	/// </summary>
	/// <param name="target">�Ώۂ̃^�[�Q�b�g</param>
	/// <returns></returns>
	Vector3 TargetVector(const Mob& target);

	/// <summary>
	/// �w�肵���^�[�Q�b�g�̕���������
	/// ���̃v���O�������Ɛ�΂ɐ��ʂ������̂ŁA������ς���̂Ɏ��Ԃ����銴���̃v���O�����ɂ�����
	/// </summary>
	/// <param name="target"></param>
	void TargetTurn(const Mob& target,const Vector3& tVec);

	void EncountSphereUpdate();

	//�ڐG���̃_���[�W
	int32_t hitDamage = 1;

	ActTable mActTable = ActTable::None;
};