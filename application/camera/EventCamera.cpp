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
	//カメラ位置をオブジェクトに読み込み
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

	//外部読み込みしたターゲットを使わないなら
	if (!useTarget)
	{
		//ターゲット位置を正面のいい感じの位置に設定
		target.position = hontai.position + centerVec * targetRadius;
	}

	//更新
	hontai.Update(*Camera::sCamera);
	target.Update(*Camera::sCamera);

	//カメラ位置をカメラ本体に読み込み
	Camera::sCamera->mEye = hontai.position;
	Camera::sCamera->mTarget = target.position;

	Camera::sCamera->UpdatematView();
}

void EventCamera::Draw()
{
	hontai.Draw();
	target.Draw();
}