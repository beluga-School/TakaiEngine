#pragma once
#include "Obj.h"
#include "Collision.h"

class Mob : public Obj3d
{
public:
	Mob();

	void Initialize();
	void Update();

	void AddGravity();

	//void CheckHitColStage(Stage& stage);
	
	/// <summary>
	/// �ړ������x�N�g���̒��o
	/// </summary>
	void PreMove();

	/// <summary>
	/// ���[���h�s��̍X�V�ƒn�ʔ���̏������ƈړ��l�̏�����
	/// </summary>
	void PostMove();

	//�󂯂�d��
	float gravity;

	//�W�����v��
	float jumpPower;
	
	//�ړ���(�b�P�ʂœ�����)
	float moveMag;

	//�n�ʂɐݒu���Ă��邩
	bool onGround = false;

	//1�t���[���ňړ�����l
	Vector3 moveValue;

	//���ʃx�N�g��
	Vector3 centerVec;

	//�������x�N�g��
	Vector3 leftVec;

	//�l�p�̓����蔻��
	Cube cubeCol;

	Ray rayCol;
};