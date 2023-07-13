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

	float GetRadius() {
		return mRadius;
	};

	enum class CamMode
	{
		Normal,
		StarGet,
	};

	CamMode GetCamMode()
	{
		return camMode;
	};

	bool CamChangeEnd() {
		return radiusMoveTimer.GetEnd();
	};

	void ChangeNormalMode();

	void ChangeStarGetMode();

private:
	PlayerCamera(){};
	~PlayerCamera(){};

	CamMode camMode = CamMode::Normal;

	//�X�^�[�擾���̃J��������
	void StarGetUpdate();

	//�ʏ�̍ۂ̃J��������
	void NormalUpdate();

	//�J�����̓����蔻����쐬
	void CreateCamCol();

	//�����̃J��������
	float mRadius = 8.0f;

	//���̃I�u�W�F�N�g�Ɠ��������I�u�W�F�N�g�𓧂�������
	Obj3d transparentObj;

	//---�X�^�[�擾���̃J�����̋�������ϐ��Q
	//���̃��f�B�E�X��ۑ�
	float saveRadius = 0.0f;

	//�C�[�W���O�̎n�_�A�I�_
	Vector3 starGetCamPosS{};
	Vector3 starGetCamPosE{};

	//�J�����𓮂����^�C�}�[
	TEasing::easeTimer camMoveTimer = 1.0f;

	//���f�B�E�X�𓮂����^�C�}�[
	TEasing::easeTimer radiusMoveTimer = 0.5f;
};

