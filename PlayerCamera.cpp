#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"
#include "ImguiManager.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();
}

void PlayerCamera::Update()
{
	//�}�E�X�Œ�
	if (Input::Keyboard::TriggerKey(DIK_N))
	{
		mouseLockChange = !mouseLockChange;	
	}
	if (mouseLockChange)
	{
		Mouse::CurserLock(CurserLockState::LOCK);
	}
	else
	{
		Mouse::CurserLock(CurserLockState::UNLOCK);
	}

	Player* player = Player::Get();

	mCenterVec = matWorld.ExtractAxisZ();
	position = player->position;

	Camera::sCamera->mEye = position - (mCenterVec * mRadius);
	Camera::sCamera->mTarget = position;

	//���f�B�E�X�ύX(�����Ă���������)
	if (Mouse::Wheel() < 0)
	{
		mRadius += 2.0f;
	}
	if (Mouse::Wheel() > 0)
	{
		mRadius -= 2.0f;
	}

	mRadius = Util::Clamp(mRadius, 1.0f, 30.f);

	//��]�����鏈��
	if (Input::Pad::CheckConnectPad())
	{
		//�c��]
		mVerticalRad += MathF::AngleConvRad(Pad::GetRStickMove().y) * mPadSensitivity;

		//����]
		mHorizontalRad += MathF::AngleConvRad(Pad::GetRStickMove().x) * mPadSensitivity;
	}

	//�}�E�X�Œ肳��Ă�Ȃ�
	if (mouseLockChange)
	{
		//�c��]
		mVerticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y) * mMouseSensitivity;

		//����]
		mHorizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x) * mMouseSensitivity;
	}

	//��ŗ��������ɓ��������Ƃ��ɉ������Ȃ��悤��clamp����

	//���E�l�𒴂��Ȃ�����
	if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
	if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);

	rotation.x = mVerticalRad;
	rotation.y = mHorizontalRad;

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	//x�̓L�����N�^�[�̉�]�ƈꏏ�ɓ�����
	//y�̓J�����I�u�W�F�N�g�œ�����
	//shift�����Ȃ���Ȃ�x���J�����I�u�W�F�N�g�œ�����

	//�J�������W�𓖂��蔻��p�ɕۑ�
	//�J�����ʒu�ƃv���C���[�ʒu�̒��S�����
	cameraCol.position = {
		(Camera::sCamera->mEye.x + player->position.x)/2,
		(Camera::sCamera->mEye.y + player->position.y)/2,
		(Camera::sCamera->mEye.z + player->position.z)/2,
	};
	
	//���S�ʒu���猳���W�ֈ����Z���ĊԂ̒������o��
	//X��Z�̕��̏o�������킩��Ȃ��̂œK���ȑ傫�������Ă���
	cameraCol.scale = {player->scale.x,
		Util::Abs(cameraCol.position.y - Camera::sCamera->mEye.y),
		player->scale.z};

	cameraCol.scale.y = Util::Clamp(cameraCol.scale.y, player->scale.y, 9999.f);
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();
}
