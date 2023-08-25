#include "EventCamera.h"
#include "MathF.h"

void EventCamera::Initialize()
{
	hontai.Initialize();
	target.Initialize();
}

void EventCamera::Update()
{
	Camera::sCamera->mEye = hontai.position;
	Camera::sCamera->mTarget = target.position;

	Camera::sCamera->UpdatematView();
}

void EventCamera::Draw()
{
	hontai.Draw();
	target.Draw();
}

void EventCamera::ObjUpdate()
{
	hontai.position = eventCamDatas.back().pos;
	hontai.rotation = {
		MathF::AngleConvRad(eventCamDatas.back().rotation.x),
		MathF::AngleConvRad(eventCamDatas.back().rotation.y),
		MathF::AngleConvRad(eventCamDatas.back().rotation.z)
	};

	Vector3 centerVec = hontai.matWorld.ExtractAxisZ();
	centerVec.normalize();

	target.position = hontai.position + centerVec * 10.0f;

	hontai.Update(*Camera::sCamera);
	target.Update(*Camera::sCamera);
}