#pragma once
#include "Entity.h"
#include "Box.h"

class Mob : public Entity
{
public:
	Vector3 moveValue = { 0,0,0 };

	std::list<IDdCube> hitListUp;
	std::list<IDdCube> hitListDown;
	std::list<IDdCube> hitListRight;
	std::list<IDdCube> hitListLeft;
	std::list<IDdCube> hitListCenter;
	std::list<IDdCube> hitListBack;
	
	Mob() {
		SetTag(TagTable::Mob);
	};

	//true�ɂȂ�Əd�͂��󂯂Ȃ�����(gravity�̒l��0�ɂ���)
	void SetNoGravity(bool flag)
	{
		if (!flag)gravity = 0;
		noGravity = flag;
	}
	void SetNoCollsion(bool flag)
	{
		mNoCollision = flag;
	}

	void SetNoMove(bool flag) {
		mNoMove = flag;
	};

	//�v���C���[�̑����̃I�u�W�F�N�g�̍��W
	float GetFeet() {
		return feet;
	};

	//�W�����v�̏�ԊǗ��X�e�[�g
	enum class JumpState
	{
		None,	//�W�����v���Ă��Ȃ�
		Up,		//�㏸��
		Staying,//�؋󎞊�
		Down,
	};

	JumpState GetJumpState() const{
		return jumpState;
	};

protected:
	bool noGravity = false;

	//�����蔻����s��Ȃ��悤�ɂ���
	bool mNoCollision = false;

	//�ړ����s���Ȃ��悤�ɂ���
	bool mNoMove = false;

	void CollsionUpdate();

	///---�c�ړ�
	JumpState jumpState = JumpState::None;

	//�㏸�C�[�W���O�̎n�_�ƏI�_
	float upJumpS = 0;
	float upJumpE = 0;

	//�W�����v��
	const float jumpPower = 5.0f;

	//�㏸�Ǘ��^�C�}�[
	TEasing::easeTimer jumpManageTimer = 0.5f;

	//�؋󎞊ԃ^�C�}�[
	TEasing::easeTimer stayManageTimer = 0.01f;

	//�d��
	float gravity = 0.0f;
	//�d�͉����x
	const float gravityAdd = 1.5f;

public:
	//���X�g�̒��ł����Ƃ��߂����W��ۑ�����ϐ�
	float hitFeetMax = 0;
	float hitCeilingMax = 0;
	float hitRightMin = 0;
	float hitLeftMin = 0;
	float hitCenterMin = 0;
	float hitBackMin = 0;
protected:

	float feet = 0;

	Vector3 initScale = { 0,0,0 };

private:
	//---�c�����̍X�V
	
	//�c�����̍X�V�܂Ƃߗp
	void UpdateY();

	void UpdateX();

	//hitlist�̒�����A�����Ƃ��߂����W��X,Y,Z�������ꂼ��ŎZ�o����֐�
	void CalcNearestHitLists();
	//�d�͂ƃW�����v�@�\�̍X�V
	void JumpUpdate();
};
