#pragma once
#include "Entity.h"
#include "Box.h"

class Mob : public Entity
{
public:
	Vector3 moveValue = { 0,0,0 };

	std::list<Cube> hitListY;

	Mob() {
		taglist.push_back(TagTable::Mob);
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

	float hitCubeMaxY = 0;

	float feet = 0;

private:
	//---�c�����̍X�V
	
	//�c�����̍X�V�܂Ƃߗp
	void UpdateY();
	//�������Ƃ̉����߂�����
	void GroundCol();
	//�d�͂ƃW�����v�@�\�̍X�V
	void JumpUpdate();
};
