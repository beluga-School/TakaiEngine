#pragma once
#include "ViewProjection.h"
#include "TEasing.h"
#include "Obj.h"
#include "Collision.h"

//�v���C���[�̃|�C���^���擾���A�����sCamera��Ǐ]������
class PlayerCamera : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

	//�J�����̌��ɂ���I�u�W�F�N�g�𓧂������鏈��
	void BackTransparent();

	static PlayerCamera* Get() {
		static PlayerCamera instance;
		return &instance;
	}

	//�J�����̍��W��ۑ�
	Cube cameraCol;

	//�c��]
	float mVerticalRad = 0;
	//����]
	float mHorizontalRad = 0;

	//�J������]�̊��x(�}�E�X)
	float mMouseSensitivity = 0.15f;

	//�J������]�̊��x(�p�b�h)
	float mPadSensitivity = 1.0f;


	//�J�����x�N�g��
	Vector3 mCenterVec = { 0,0,0 };

	bool mouseLockChange = true;

private:
	PlayerCamera(){};
	~PlayerCamera(){};

	float mRadius = 5.0f;

	//���̃I�u�W�F�N�g�Ɠ��������I�u�W�F�N�g�𓧂�������
	Obj3d transparentObj;
};

