#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();
}

void PlayerCamera::Update()
{
	//�}�E�X�Œ�
	Mouse::CurserLock(CurserLockState::LOCK);

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
	//�c��]
	mVerticalRad += MathF::AngleConvRad(Mouse::GetVelocity().y) * mSensitivity;
	//���E�l�𒴂��Ȃ�����
	if (mVerticalRad > MathF::PIf / 2 - MathF::AngleConvRad(1.0f)) mVerticalRad = MathF::PIf / 2 - MathF::AngleConvRad(1.0f);
	if (mVerticalRad < -MathF::PIf / 2 + MathF::AngleConvRad(1.0f)) mVerticalRad = -MathF::PIf / 2 + MathF::AngleConvRad(1.0f);
	rotation.x = mVerticalRad;

	//����]
	mHorizontalRad += MathF::AngleConvRad(Mouse::GetVelocity().x) * mSensitivity;
	rotation.y = mHorizontalRad;

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	//x�̓L�����N�^�[�̉�]�ƈꏏ�ɓ�����
	//y�̓J�����I�u�W�F�N�g�œ�����
	//shift�����Ȃ���Ȃ�x���J�����I�u�W�F�N�g�œ�����
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();
}
