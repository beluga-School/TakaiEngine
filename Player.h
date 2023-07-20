#pragma once
#include "TEasing.h"
#include "CollideManager.h"
#include "Status.h"

class Player : public Mob
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

	//�G�Ƃ̓����蔻��p�X�t�B�A
	Sphere playerCol;

	//�ĂԂ�1��W�����v����
	void Jump();

private:
	Status hp = 8;

	Player(){};
	~Player(){};

	void Attack();

	void SideMoveUpdate();
	void ColUpdate();
	void RotaUpdate();

	///---���ړ�
	//�ړ����x
	float mSpeed = 7.5f;

	//�ő�l�ɂȂ�܂ł̉�������
	TEasing::easeTimer accelerationTimer = 0.25f;

	//�ő�l���猸��܂ł̌�������
	TEasing::easeTimer decelerationTimer = 0.25f;

	//�ړ��I��������̑��x
	float mSaveSpeed = 0.0f;

	//�ő呬�x
	const float MAX_SPEED = 7.5f;

	//�_�b�V�����̍ő呬�x
	const float MAX_DASH_SPEED = 15.0f;

	Vector3 oldMoveVec = {0,0,0};

	//�����x�N�g���ۑ�
	Vector3 mCenterVec = {0,0,0};
	Vector3 mSideVec = {0,0,0};

	//�G�Ƃ̓����蔻��p�X�t�B�A�̕`��
	Obj3d colDrawer;

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

	//---�X�^�[�擾
	enum class StarState
	{
		None,	//���擾
		Get,	//���胂�[�V������
	}starState = StarState::None;
};