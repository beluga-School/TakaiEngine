#pragma once
#include "EventBlock.h"

class Star final : public EventBlock
{
public:
	Star() : EventBlock()
	{
		SetTag(TagTable::Star);
	}

	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;

	//�X�e�[�g��CountUp�܂œ��B���Ă�����AEnd�ɕύX����
	void StateEnd();

	bool InHoleEnd();

	enum class StarState
	{
		None,
		jumpUp,
		Inhole,
		CountUp,
		End,
	};

	//���݂̏�Ԃ��擾
	StarState GetState();

private:
	StarState starState = StarState::None;

	//���莞�ɕۑ�����X�P�[��
	Vector3 saveScale = { 0,0,0 };

	//���莞�Ƀv���C���[�ɋz�����܂��C�[�W���O�̎n�_�ƏI�_
	Vector3 saveStartPos = { 0,0,0 };
	Vector3 savePlayerPos = { 0,0,0 };

	Vector3 jumpUpPos = { 0,0,0 };
	Vector3 jumpUpScale = { 0,0,0 };

	//1�b�Ԃ̉�]��
	float rotaSpeed = 0.25f;

	//��яオ�鎞��
	TEasing::easeTimer jumpUpTimer = 1.0f;
	//�󒆂łƂǂ܂鎞��
	TEasing::easeTimer delayTimer = 0.2f;
	//�z�����܂�鎞��
	TEasing::easeTimer inholeTimer = 2.0f;

	//�p�[�e�B�N���̔����^�C�}�[
	TEasing::easeTimer particleTimer = 1.0f;
};