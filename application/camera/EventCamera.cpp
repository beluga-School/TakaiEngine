#include "EventCamera.h"
#include "MathF.h"
#include "EventManager.h"

void EventCamera::Initialize()
{
	useTarget = false;
	hontai.Initialize();
	target.Initialize();
}

void EventCamera::SetPos(const Vector3& position)
{
	//�J�����ʒu���I�u�W�F�N�g�ɓǂݍ���
	hontai.position = position;
}

void EventCamera::SetTarget(const Vector3& targetpos)
{
	target.position = targetpos;
	useTarget = true;
}

void EventCamera::SetRotation(const Vector3& rotation)
{
	hontai.rotation = {
		MathF::AngleConvRad(rotation.x),
		MathF::AngleConvRad(rotation.y),
		MathF::AngleConvRad(rotation.z)
	};
}

void EventCamera::Update()
{
	Vector3 centerVec = hontai.matWorld.ExtractAxisZ();
	centerVec.normalize();

	//�O���ǂݍ��݂����^�[�Q�b�g���g��Ȃ��Ȃ�
	if (!useTarget)
	{
		//�^�[�Q�b�g�ʒu�𐳖ʂ̂��������̈ʒu�ɐݒ�
		target.position = hontai.position + centerVec * targetRadius;
	}

	//�X�V
	hontai.Update(*Camera::sCamera);
	target.Update(*Camera::sCamera);

	//�J�����ʒu���J�����{�̂ɓǂݍ���
	Camera::sCamera->mEye = hontai.position;
	Camera::sCamera->mTarget = target.position;

	Camera::sCamera->UpdatematView();
}

void EventCamera::Draw()
{
	hontai.Draw();
	target.Draw();
}