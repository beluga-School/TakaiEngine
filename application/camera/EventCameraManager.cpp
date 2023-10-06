#include "EventCameraManager.h"
#include "ViewProjection.h"
#include <EventManager.h>

bool EventCameraManager::SetEventCamera(const std::string& eventname)
{
	//イベントが見つかんなかったら失敗で返す
	if (eventCameraDatas.find(eventname) == eventCameraDatas.end()) return false;

	eventCamera.Initialize();

	//イベント名を保持
	eventname_ = eventname;

	//カメラが複数あるなら
	if (eventCameraDatas[eventname].datas.size() > 1)
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
	frontCamera = &eventCameraDatas[eventname].datas.front();
	backCamera = &eventCameraDatas[eventname].datas.front() + itrNumber;

	eventPositions.clear();
	for (auto& camdata : eventCameraDatas[eventname].datas)
	{
		eventPositions.push_back(camdata.pos);
	}

	//ターゲットデータが何かしら入っているなら
	if (eventCameraDatas[eventname].InTargetData())
	{
		//ターゲットデータをセット
		eventCamera.SetTarget(eventCameraDatas[eventname_].target);
	}

	return true;
}

bool EventCameraManager::GetEventEnd()
{
	if (!eventCameraDatas[eventname_].InTargetData())
	{
		return moveTimer.GetEnd() &&  itrNumber >= eventCameraDatas[eventname_].datas.size() - 1;
	}
	else
	{
		return moveTimer.GetEnd();
	}
}

void EventCameraManager::Start()
{
	moveTimer.Start();

	rotaTimer.Start();
	rotaTimer.mMaxTime = moveTimer.mMaxTime / eventCameraDatas[eventname_].datas.size();
}

void EventCameraManager::Update()
{
	moveTimer.Update();

	eventCamera.SetPos(Vector3::Spline(eventPositions, moveTimer.GetTimeRate()));
	
	eventCamera.Update();

	if (!eventCameraDatas[eventname_].InTargetData())
	{
		rotaTimer.Update();
		
		eventCamera.SetRotation(TEasing::InQuad(frontCamera->rotation, backCamera->rotation, rotaTimer.GetTimeRate()));
		if (rotaTimer.GetEnd())
		{
			//管理番号が全体のサイズより小さいなら
			if (itrNumber < eventCameraDatas[eventname_].datas.size() - 1)
			{
				//番号を増やして
				itrNumber++;
				frontCamera = backCamera;
				backCamera = &eventCameraDatas[eventname_].datas.front() + itrNumber;
				rotaTimer.Start();
			}
		}
	}

	DebugGUI();
}

void EventCameraManager::Register(std::string string, EventCamManageData datas, float movetime)
{
	eventCameraDatas[string] = datas;
	moveTimer.mMaxTime = movetime;
}

void EventCameraManager::DebugGUI()
{
	hoge.Begin({200,200},{500,300});
	ImGui::Text("frontCamera pos.x:%f,y:%f,z:%f", frontCamera->pos.x, frontCamera->pos.y, frontCamera->pos.z);
	ImGui::Text("backCamera pos.x:%f,y:%f,z:%f", backCamera->pos.x, backCamera->pos.y, backCamera->pos.z);
	ImGui::Text("eventCameraDatas.size:%d", (int32_t)eventCameraDatas.size());
	ImGui::Text("moveTimer:%f", moveTimer.GetTimeRate());
	hoge.End();
}

void EventCameraManager::Reset()
{
	frontCamera = nullptr;
	backCamera = nullptr;
	eventCameraDatas.clear();
	moveTimer.Reset();
	rotaTimer.Reset();
}

bool EventCamManageData::InTargetData()
{
	//ターゲットの値が初期値でないなら入っていると判定
	return target.x != -1000 &&target.y != -1000 && target.z != -1000;
}
