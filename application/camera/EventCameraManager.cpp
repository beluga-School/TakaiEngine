#include "EventCameraManager.h"
#include "Camera.h"
#include <EventManager.h>

bool EventCameraManager::SetEventCamera(const std::string& eventname)
{
	//イベントが見つかんなかったら失敗で返す
	if (eventCameraDatas.find(eventname) == eventCameraDatas.end()) return false;

	eventCamera.Initialize();

	//イベント名を保持
	eventname_ = eventname;

	nowCamEvent = &eventCameraDatas[eventname];

	//カメラが複数あるなら
	if (nowCamEvent->datas.size() > 1)
	{
		itrNumber = 1;
	}
	//そうでないなら
	else
	{
		//手前と後ろに同じものをセットしておく
		itrNumber = 0;
	}

	//番号が若い二つをセットする
	frontCamera = &nowCamEvent->datas.front();
	backCamera = &nowCamEvent->datas.front() + itrNumber;

	eventPositions.clear();
	for (auto& camdata : nowCamEvent->datas)
	{
		eventPositions.push_back(camdata.pos);
	}

	//ターゲットデータが何かしら入っているなら
	if (nowCamEvent->InTargetData())
	{
		//ターゲットデータをセット
		eventCamera.SetTarget(nowCamEvent->target);
	}

	return true;
}

EventCamManageData* EventCameraManager::GetNowEventCameraData()
{
	return nowCamEvent;
}

bool EventCameraManager::GetEventEnd()
{
	if (!nowCamEvent->InTargetData())
	{
		return nowCamEvent->endStandbyTimer.GetEnd() &&  itrNumber >= nowCamEvent->datas.size() - 1;
	}
	else
	{
		return nowCamEvent->endStandbyTimer.GetEnd();
	}
}

void EventCameraManager::Start()
{
	if (nowCamEvent == nullptr)return;

	nowCamEvent->Start();
}

void EventCameraManager::Update()
{
	nowCamEvent->Update();

	eventCamera.SetPos(Vector3::Spline(eventPositions, nowCamEvent->moveTimer.GetTimeRate()));
	
	eventCamera.Update();

	if (!nowCamEvent->InTargetData())
	{
		nowCamEvent->mRotaTimer.Update();
		
		eventCamera.SetRotation(TEasing::InQuad(frontCamera->rotation, backCamera->rotation,
			nowCamEvent->mRotaTimer.GetTimeRate()));
		if (nowCamEvent->mRotaTimer.GetEnd())
		{
			//管理番号が全体のサイズより小さいなら
			if (itrNumber < nowCamEvent->datas.size() - 1)
			{
				//番号を増やして
				itrNumber++;
				frontCamera = backCamera;
				backCamera = &nowCamEvent->datas.front() + itrNumber;
				nowCamEvent->mRotaTimer.Start();
			}
		}
	}

	//DebugGUI();
}

void EventCameraManager::Register(std::string string, EventCamManageData datas)
{
	eventCameraDatas[string] = datas;
}

void EventCameraManager::DebugGUI()
{
	if (nowCamEvent == nullptr)return;

	mRotCheck.Begin({200,200},{500,300});
	ImGui::Text("frontCamera pos.x:%f,y:%f,z:%f", frontCamera->pos.x, frontCamera->pos.y, frontCamera->pos.z);
	ImGui::Text("backCamera pos.x:%f,y:%f,z:%f", backCamera->pos.x, backCamera->pos.y, backCamera->pos.z);
	ImGui::Text("eventCameraDatas.size:%d", (int32_t)eventCameraDatas.size());
	ImGui::Text("moveTimer:%f", nowCamEvent->moveTimer.GetTimeRate());
	mRotCheck.End();
}

void EventCameraManager::Reset()
{
	if (nowCamEvent == nullptr)return;

	frontCamera = nullptr;
	backCamera = nullptr;
	eventCameraDatas.clear();
	nowCamEvent->moveTimer.Reset();
	nowCamEvent->mRotaTimer.Reset();
}

bool EventCamManageData::InTargetData()
{
	//ターゲットの値が初期値でないなら入っていると判定
	return target.x != -1000 &&target.y != -1000 && target.z != -1000;
}

void EventCamManageData::SetTimersInfo(float moveMax, float startStandbyMax, float endStandbyMax)
{
	moveTimer.mMaxTime = moveMax;
	startStandbyTimer.mMaxTime = startStandbyMax;
	endStandbyTimer.mMaxTime = endStandbyMax;
}

void EventCamManageData::Start()
{
	startStandbyTimer.Start();

	endStandbyTimer.Reset();

	moveTimer.Reset();
	
	mRotaTimer.Reset();
	mRotaTimer.mMaxTime = moveTimer.mMaxTime / datas.size();
}

void EventCamManageData::Update()
{
	moveTimer.Update();
	startStandbyTimer.Update();
	endStandbyTimer.Update();

	if (startStandbyTimer.GetEnd())
	{
		if(!moveTimer.GetStarted())	moveTimer.Start();
		if(!mRotaTimer.GetStarted())	mRotaTimer.Start();
	}
	if (moveTimer.GetEnd())
	{
		if (!endStandbyTimer.GetStarted())endStandbyTimer.Start();
	}
}
