#pragma once
#include "EventBlock.h"

class Star : public EventBlock
{
public:
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void HitEffect()override;

	bool InHoleEnd() {
		return timer.GetEnd();
	}

	enum class StarState
	{
		None,
		Inhole,
		End,
	}starState = StarState::None;

private:

	//���莞�ɕۑ�����X�P�[��
	Vector3 saveScale = { 0,0,0 };

	//���莞�Ƀv���C���[�ɋz�����܂��C�[�W���O�̎n�_�ƏI�_
	Vector3 saveStartPos = { 0,0,0 };
	Vector3 savePlayerPos = { 0,0,0 };

	//1�b�Ԃ̉�]��
	float rotaSpeed = 0.25f;

	TEasing::easeTimer timer = 2.0f;

	TEasing::easeTimer particleTimer = 1.0f;
};

class StarManager
{
public:
	//�X�^�[�̓v���C���[�Ƃ̌��т��������̂ŁA�ʂŊǗ�����悤��
	//����X�e�[�W���ƂɁA�X�e�[�W�f�[�^�Ƃ͕ʂ̃t�@�C���Ő��̎擾�󋵂Ƃ����Ǘ�����p�ɂ��悤
	std::list<std::unique_ptr<Star>> mStars;

	//���擾���o�̐i�s�Ǘ�
	enum class StarGetState
	{
		None,	//�����Ȃ�
		MoveCam,//�J����������
		Inhole,	//�v���C���[�ɋz�����܂��
		Jumping,//�v���C���[���W�����v����
		BackCam,//�J���������ɖ߂�
	}progress = StarGetState::None;

	static StarManager* Get() {
		static StarManager instance;
		return &instance;
	};

	void Start() {
		progress = StarGetState::MoveCam;
		jumpingCount = 0;
	};

	void JumpMove();

	void Update();

private:
	StarManager(){};
	~StarManager(){};

	TEasing::easeTimer jumpingTimer = 1.0f;
	int32_t jumpingCount = 0;
};
