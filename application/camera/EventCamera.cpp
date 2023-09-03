#include "EventCamera.h"
#include "MathF.h"
#include "EventManager.h"

void EventCamera::Initialize()
{
	hontai.Initialize();
	target.Initialize();
}

void EventCamera::Update()
{
	//�J�����ʒu���I�u�W�F�N�g�ɓǂݍ���
	hontai.position = eventCamData.pos;
	hontai.rotation = {
		MathF::AngleConvRad(eventCamData.rotation.x),
		MathF::AngleConvRad(eventCamData.rotation.y),
		MathF::AngleConvRad(eventCamData.rotation.z)
	};

	Vector3 centerVec = hontai.matWorld.ExtractAxisZ();
	centerVec.normalize();

	//�^�[�Q�b�g�ʒu��K�p
	target.position = hontai.position + centerVec * targetRadius;

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