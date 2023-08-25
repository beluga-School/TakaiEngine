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
	//カメラ位置をオブジェクトに読み込み
	hontai.position = eventCamData.pos;
	hontai.rotation = {
		MathF::AngleConvRad(eventCamData.rotation.x),
		MathF::AngleConvRad(eventCamData.rotation.y),
		MathF::AngleConvRad(eventCamData.rotation.z)
	};

	Vector3 centerVec = hontai.matWorld.ExtractAxisZ();
	centerVec.normalize();

	//ターゲット位置を適用
	target.position = hontai.position + centerVec * targetRadius;

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

void EventCameraManager::Update()
{
	for (auto& camera : eventCameras)
	{
		//イベント番号が一致したらそのカメラを実行
		if (EventManager::Get()->GetNowEvent() == camera.eventCamData.eventName)
		{
			camera.Update();
		}
	}
}

void EventCameraManager::Draw()
{
	for (auto& camera : eventCameras)
	{
		camera.Draw();
	}
}

void EventCameraManager::Register(const EventCamData& eventCamData)
{
	eventCameras.emplace_back();
	eventCameras.back().eventCamData = eventCamData;
	eventCameras.back().Initialize();
}

void EventCameraManager::Clear()
{
	eventCameras.clear();
}
