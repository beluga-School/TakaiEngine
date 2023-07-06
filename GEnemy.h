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
		Traking,	//�ǂ������Ă�����
		Attacking,	//�U�����̏��(���̏�Ԃ̏I���ɍU�����肪�ł�)
		Staying,	//�U����̌㌄ ��ԏI���œ����蔻����Ƃ�ANone��Encount�ɑJ��
	}state = State::None;

	float mSpeed = 2.5f;	//1�b�Ԃ̈ړ����x

	//�������̃W�����v�Ǘ��^�C�}�[
	TEasing::easeTimer encountJumpTimer = 0.5f;
	float encountJumpS = 0;
	float encountJumpE = 0;

	//�ǐՎ��ԊǗ��^�C�}�[(�I�������ēx�����蔻����Ƃ�A�������ĂȂ��Ȃ�ǐՂ��I������)
	TEasing::easeTimer trackingTimer = 2.0f;

	//�ǐՎ��̉��h��Ǘ��^�C�}�[
	TEasing::easeTimer metronomeTimer = 0.5f;
	//�U���܂ł̎���
	TEasing::easeTimer attackTimer = 1.0f;
	//�U����̌㌄����
	TEasing::easeTimer stayTimer = 1.0f;
};