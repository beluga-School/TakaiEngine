#pragma once
#include "Obj.h"
#include "Enemy.h"
#include "Collision.h"
#include "TEasing.h"
#include "MathF.h"

class GEnemy : public Enemy
{
public:
	void Initialize();
	void Update();
	void Draw();

	void HitEffect();

	void Encount();

	void ColUpdate();

	bool IsDead()
	{
		return attackState == AttackState::Dead;
	}

private:
	Obj3d hitSphere;

	enum class AttackState
	{
		None,		//�����~�܂��Ă���
		Encount,	//�v���C���[�𔭌��������
		Attacking,	//�ːi���
		Staying,	//�U����̌㌄ ��ԏI���œ����蔻����Ƃ�ANone��Encount�ɑJ��
		Dead,
	}attackState = AttackState::None;

	float mSpeed = 2.5f;	//1�b�Ԃ̈ړ����x

	//�������̃W�����v�Ǘ��^�C�}�[
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//�ǐՎ��ԊǗ��^�C�}�[(�I�������ēx�����蔻����Ƃ�A�������ĂȂ��Ȃ�ǐՂ��I������)
	Vector3 attackPosS = {0,0,0};
	Vector3 attackPosE = {0,0,0};
	float attackDistance = 2.0f;

	//�v���C���[�ւ̃x�N�g��
	Vector3 pVec{};

	TEasing::easeTimer accelerationTimer = 0.5f;

	//�����̍ő�l
	const float MAX_ACCELERATION = 6.0f;

	//�ǐՎ��̉��h��Ǘ��^�C�}�[
	TEasing::easeTimer metronomeTimer = 0.5f;
	//�U����̌㌄����
	TEasing::easeTimer stayTimer = 1.0f;

	//���S��̐�����΂��ꉉ�o�Ŏg�����
	//������΂���C�[�W���O�̎n�_�ƏI�_
	Vector3 deadEasingS{};
	Vector3 deadEasingE{};

	TEasing::easeTimer deadTimer = 0.5f;

	//������΂��ꂽ���̕���
	Vector3 deadDirection;

	//���S��̐�����΂��ꒆ�̉�]��(1�b��)(���W�A��)
	float deadRoring = MathF::PIf * 10;

	//�d��
	float gravity = 0;
	const float gravityAdd = 0.1f;
};