#pragma once
#include "Obj.h"
#include "Collision.h"
#include "CollideManager.h"

class Enemy : public Mob
{
public:
	Enemy() : Mob()
	{
		
	}
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void HitEffect() = 0;

	virtual void Encount() = 0;

	Sphere sphereCol{};

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
};