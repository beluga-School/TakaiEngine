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

	//�v���C���[�̑����̃I�u�W�F�N�g�̍��W
	float GetFeet() {
		return feet;
	};

protected:
	bool noGravity = false;

	//�c�����̍X�V
	void UpdateY();

	///---�c�ړ�
	enum class JumpState
	{
		None,	//�W�����v���Ă��Ȃ�
		Up,		//�㏸��
		Staying,//�؋󎞊�
		Down,
	}jumpState = JumpState::None;

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
	//�������Ƃ̉����߂�����
	void GroundCol();
	//�d�͂ƃW�����v�@�\�̍X�V
	void JumpUpdate();
};
