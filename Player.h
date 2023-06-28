#pragma once
#include "Mob.h"
#include "TEasing.h"

class Player : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

private:
	///---���ړ�
	//�ړ����x
	float mSpeed = 7.5f;

	//�����x�N�g���ۑ�
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//�ړ��l(���ړ�)
	Vector3 moveValue = {0,0,0};

	//�O�ړ�(��ړ�)
	Vector3 preMove = {0,0,0};

	///---�c�ړ�
	enum class JumpState
	{
		None,	//�W�����v���Ă��Ȃ�
		Up,		//�㏸��
		Staying,//�؋󎞊�
		Down,	//���~��
		End,	//�I���𖾎��I��
	};
	JumpState jumpState = JumpState::None;

	std::list<Cube> hitList;

	//�㏸�C�[�W���O�̎n�_�ƏI�_
	float upJumpS = 0;
	float upJumpE = 0;

	//���~�C�[�W���O�̎n�_�ƏI�_
	float downJumpS = 0;
	float downJumpE = 0;

	//�W�����v��
	const float jumpPower = 10.0f;

	//�㏸�Ǘ��^�C�}�[
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//�؋󎞊ԃ^�C�}�[
	TEasing::easeTimer stayManageTimer = 0.1f;

	//�d��
	float gravity = 0.0f;
	//�d�͉����x
	const float gravityAdd = 0.1f;

	///---�����蔻��
	//���������������蔻������I�u�W�F�N�g���Ǘ����郊�X�g�������ɂ��������������C������
};

