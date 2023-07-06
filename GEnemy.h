#pragma once
#include "Obj.h"
#include "Enemy.h"
#include "Collision.h"
#include "TEasing.h"

class GEnemy : public Enemy
{
public:
	void Initialize();
	void Update();
	void Draw();

	void HitEffect();

	void Encount();

	void ColUpdate();

private:
	Obj3d hitSphere;

	enum class State
	{
		None,		//�����~�܂��Ă���
		Encount,	//�v���C���[�𔭌��������
		Attacking,	//�ːi���
		Staying,	//�U����̌㌄ ��ԏI���œ����蔻����Ƃ�ANone��Encount�ɑJ��
	}state = State::None;

	float mSpeed = 2.5f;	//1�b�Ԃ̈ړ����x

	//�������̃W�����v�Ǘ��^�C�}�[
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//�ǐՎ��ԊǗ��^�C�}�[(�I�������ēx�����蔻����Ƃ�A�������ĂȂ��Ȃ�ǐՂ��I������)
	TEasing::easeTimer attakingTimer = 3.0f;
	Vector3 attackPosS = {0,0,0};
	Vector3 attackPosE = {0,0,0};
	float attackDistance = 2.0f;

	//�v���C���[�ւ̃x�N�g��
	Vector3 pVec{};

	//�U���̉����x
	float acceleration = 0.1f;

	//�����̍ő�l
	const float MAX_ACCELERATION = 30.0f;

	//�ǐՎ��̉��h��Ǘ��^�C�}�[
	TEasing::easeTimer metronomeTimer = 0.5f;
	//�U����̌㌄����
	TEasing::easeTimer stayTimer = 1.0f;

	//�^�[�Q�b�g�}�[�N��1��]�̎���
	TEasing::easeTimer markRotaTimer = 0.5f;

	//�U���O�̌x���}�[�N���f��
	Obj3d targetMark;
};