#pragma once
#include "ViewProjection.h"
#include "TEasing.h"
#include "Obj.h"

//�v���C���[�̃|�C���^���擾���A�����sCamera��Ǐ]������
class PlayerCamera : public Obj3d
{
public:
	void Initialize();
	void Update();
	void Draw();

	static PlayerCamera* Get() {
		static PlayerCamera instance;
		return &instance;
	}

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

};

