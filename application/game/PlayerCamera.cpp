#include "PlayerCamera.h"
#include "Player.h"
#include "MathF.h"
#include "Input.h"
#include "Util.h"
#include "ImguiManager.h"
#include "Stage.h"
#include "Star.h"
#include "EventManager.h"

using namespace Input;

void PlayerCamera::Initialize()
{
	Camera::sCamera->Initialize();
	Obj3d::Initialize();

	transparentObj.Initialize();
	transparentObj.SetModel(ModelManager::GetModel("BlankCube"));
	transparentObj.scale = { 5,5,5 };
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

	switch (camMode)
	{
	case PlayerCamera::CamMode::Normal:
		NormalUpdate();
		break;
	case PlayerCamera::CamMode::StarGet:
		StarGetUpdate();
		break;
	}

	Camera::sCamera->UpdatematView();
	Obj3d::Update(*Camera::sCamera);

	CreateCamCol();
}

void PlayerCamera::Draw()
{
	Obj3d::Draw();
}

void PlayerCamera::BackTransparent()
{
	Player* player = Player::Get();

	//X��Z���v���C���[�̌����~�^���ł��������ɒǏ]����
	Vector2 invObjNonY = MathF::CircularMotion({ player->position.x,player->position.z },
		(transparentObj.scale.x + transparentObj.scale.z) / 2 * 0.8f,
		-player->rotation.y - MathF::PIf / 2);

	//Y�̓J�����ʒu�Ɠ����ɂ���
	transparentObj.position = { invObjNonY.x,Camera::sCamera->mEye.y,invObjNonY.y };

	transparentObj.Update(*Camera::sCamera);

	//���������I�u�W�F�N�g�̕`��t���O��܂�
	for (auto& obj : StageChanger::Get()->mEntitys)
	{
		//�u���b�N�ȊO�Ȃ玟��
		if (!obj.CheckTag(TagTable::Block))continue;
		
		//�u���b�N�Ȃ̂��m�肵���̂Ńu���b�N�^�ɕϊ�
		Block* block = static_cast<Block*>(&obj);

		//�������Ă��������
		if (Collsions::BoxColAABB(block->box, transparentObj))
		{
			//�n�ʂ������Ăق����Ȃ��̂ŁA�n�ʂ̍��W���v���C���[�̑�����艺�Ȃ�
			//�������ĂĂ������鏈�����X�L�b�v����
			if (block->box.position.y + block->box.scale.y / 2 <= player->GetFeet())
			{
				continue;
			}

			if (block->transparentTimer.GetStarted() == false)
			{
				block->transparentTimer.Start();
			}
			else if (block->transparentTimer.GetReverseStarted())
			{
				block->transparentTimer.Reset();
			}
			//�A�E�g���C���͂����ɏ���
			block->SetOutLineAlpha(0.0f);
		}
		//�������ĂȂ��Ȃ�i�X�Z������
		else
		{
			//�^�C�}�[��TimeRate��0.0~1.0�͈̔͂œ����̂ŁA���̒l�𔽓]���������̂�Alpha�Ƃ��Ĉ���
			block->transparentTimer.ReverseStart();
			//�A�E�g���C���͂����ɖ߂�
			block->SetOutLineAlpha(1.0f);
		}
		//�i�X����������Z�������肷��
		block->color_.w = 1.0f - block->transparentTimer.GetTimeRate();
	}
}

void PlayerCamera::ChangeNormalMode()
{
	camMode = CamMode::Normal;

	camMoveTimer.Reset();
	mRadius = saveRadius;
}

void PlayerCamera::ChangeStarGetMode()
{
	//���łɊJ�n����Ă�Ȃ�X�L�b�v
	if (camMode == CamMode::StarGet)return;

	Player* player = Player::Get();

	camMode = CamMode::StarGet;
	camMoveTimer.Start();
	radiusMoveTimer.Reset();

	//���݈ʒu���n�_��
	starGetCamPosS = Camera::sCamera->mEye;

	//�v���C���[�̐��ʍ��W���Z�o
	mCenterVec = Player::Get()->matWorld.ExtractAxisZ();
	//y�����𔽑΂ɂ��ď�ɍs���悤��
	mCenterVec.y = 0.25f;

	position = player->position;

	//�v���C���[�̐��ʍ��W���I�_��
	starGetCamPosE = position + (mCenterVec * mRadius);

	saveRadius = mRadius;
};

void PlayerCamera::StarGetUpdate()
{
	Player* player = Player::Get();

	camMoveTimer.Update();
	radiusMoveTimer.Update();

	Camera::sCamera->mTarget = position;

	Camera::sCamera->mEye = TEasing::InQuad(starGetCamPosS, starGetCamPosE, camMoveTimer.GetTimeRate());

	if (camMoveTimer.GetEnd())
	{
		if(radiusMoveTimer.GetStarted() == false)radiusMoveTimer.Start();
		mRadius = TEasing::InQuad(saveRadius,4.0f, radiusMoveTimer.GetTimeRate());

		position = player->position;

		Camera::sCamera->mEye = position + (mCenterVec * mRadius);
	}
}

void PlayerCamera::NormalUpdate()
{
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
}

void PlayerCamera::CreateCamCol()
{
	Player* player = Player::Get();

	//�J�������W�𓖂��蔻��p�ɕۑ�
	//�J�����ʒu�ƃv���C���[�ʒu�̒��S�����
	cameraCol.position = {
		(Camera::sCamera->mEye.x + player->position.x) / 2,
		(Camera::sCamera->mEye.y + player->position.y) / 2,
		(Camera::sCamera->mEye.z + player->position.z) / 2,
	};

	//���S�ʒu���猳���W�ֈ����Z���ĊԂ̒������o��
	//X��Z�̕��̏o�������킩��Ȃ��̂œK���ȑ傫�������Ă���
	cameraCol.scale = { player->scale.x,
		Util::Abs(cameraCol.position.y - Camera::sCamera->mEye.y),
		player->scale.z };

	cameraCol.scale.y = Util::Clamp(cameraCol.scale.y, player->scale.y, 9999.f);
}
