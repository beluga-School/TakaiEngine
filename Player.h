#pragma once
#include "Obj.h"
#include "Mob.h"
#include "Input.h"
#include "Stage.h"
#include "Vector2.h"

class Player : public Mob
{
public:
	Player() {
		Initialize();
	};

	void Respawn();

	void Initialize();

	//�X�e�[�W�Ƃ̓����蔻����s�����߁A�X�e�[�W�������Ŗ���Ă���
	//�X�e�[�W�͂ǂ����ЂƂȂ̂ŁA���݂̃X�e�[�W���V���O���g���ŖႦ��Ƃ������Y�킩��
	//(nowCamera�Ɠ������o�ŁAnowStage�I��)
	void Update(const Stage& stage);

	void Draw();

	bool isWallGrap = false;
	bool isDash = false;
	float dashTimer = 0;
	Vector3 dashVec = { 0,0,0 };

	//Vector3 spawnPos = { 0,330,-560 };
	Vector3 spawnPos = { 0,0,0 };

	float horizontalRotation;
	float verticalRotation;

	POINT point;

	//�}�E�X�̊��x
	float mouseSpd = 0.0025f;

	float dashCool = 0;

	bool oldGround = false;

	int hp = 0;
	bool isDead = false;

	float mutekiTimer = 0;
	int blinkTimer = 0;

	Vector2 max;

	void HitEffect();

private:
	///---�N���X���Ŏg�p����֐��Q

	//���ړ�
	void Move();

	//�c�ړ�(�W�����v)
	void Jump();

	///---�V���O���g��
	Input* input = Input::GetInstance();
};