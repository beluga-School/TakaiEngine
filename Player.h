#pragma once
#include "Mob.h"
#include "TEasing.h"

//�v���C���[���ʂŎ��@�\
//����
//�J����

//�ĉ������Ǎ�肪�Ⴄ�@�\
//�W�����v
//�ړ�
//��]
//�ĉ��ō��������@�\
//�����蔻��

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

class Player : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

	void Reset();

	static Player* Get()
	{
		static Player instance;
		return &instance;
	}

	//�v���C���[�̉�]���Ǘ�����ϐ�
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	//�O�ړ�(��ړ�)
	Vector3 preMove = { 0,0,0 };

private:
	Player(){};
	~Player(){};

	void Attack();

	void SideMoveUpdate();
	void JumpUpdate();
	void ColUpdate();
	void RotaUpdate();

	///---���ړ�
	//�ړ����x
	float mSpeed = 7.5f;

	//�����x�N�g���ۑ�
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//�ړ��l(���ړ�)
	Vector3 moveValue = {0,0,0};

	///---�c�ړ�
	enum class JumpState
	{
		None,	//�W�����v���Ă��Ȃ�
		Up,		//�㏸��
		Staying,//�؋󎞊�
	}jumpState = JumpState::None;

	//�㏸�C�[�W���O�̎n�_�ƏI�_
	float upJumpS = 0;
	float upJumpE = 0;

	//�W�����v��
	const float jumpPower = 10.0f;

	//�㏸�Ǘ��^�C�}�[
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//�؋󎞊ԃ^�C�}�[
	TEasing::easeTimer stayManageTimer = 0.1f;

	//�d��
	float gravity = 0.0f;
	//�d�͉����x
	const float gravityAdd = 1.5f;

	///---�����蔻��
	//���������������蔻������I�u�W�F�N�g���Ǘ����郊�X�g�������ɂ��������������C������
	std::list<Cube> hitListX;
	std::list<Cube> hitListY;
	
	float hitCubeMaxY = 0;

	///---�U��
	enum class AttackState
	{
		None,		//�U�����Ă��Ȃ�
		Attacking,	//�U����
		CoolTime,	//�N�[���^�C��
	}attackState = AttackState::None;

	//���[�����O���Ԃ̊Ǘ�
	TEasing::easeTimer attackingTimer = 1.0f;
	//���[�����O�̉�]���[�V�����̊Ǘ�
	TEasing::easeTimer attackingMotionTimer = 0.5f;
	//���[�����O�̃N�[���^�C���̊Ǘ�
	TEasing::easeTimer attackCoolTimer = 0.75f;

	//���[�����O�̉�]���̊Ǘ�
	int32_t mRolingNum = 3;

	Vector3 mRolingVec = {0,0,0};

	float mRolingSpeed = 15.0f;
};

