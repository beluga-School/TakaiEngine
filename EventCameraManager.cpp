#include "EventCameraManager.h"
#include "ViewProjection.h"
#include <EventManager.h>

bool EventCameraManager::SetEventCamera(const std::string& eventname)
{
	//イベントが見つかんなかったら失敗で返す
	if (eventCameraDatas.find(eventname) == eventCameraDatas.end()) return false;

	//カメラが複数あるなら
	if (eventCameraDatas[eventname].size() > 1)
	{
		//番号が若い二つをセットする
		frontCamera = &eventCameraDatas[eventname].front();
		backCamera = &eventCameraDatas[eventname].front() + 1;
	}
	//そうでないなら
	else
	{
		//手前と後ろに同じものをセットしておく
		frontCamera = &eventCameraDatas[eventname].front();
		backCamera = &eventCameraDatas[eventname].front();
	}

	eventCamera.Initialize();
	return true;
}

void EventCameraManager::Start()
{
	moveTimer.Start();
}

void EventCameraManager::Update()
{
	moveTimer.Update();

	eventCamera.SetPos(TEasing::InQuad(frontCamera->pos, backCamera->pos, moveTimer.GetTimeRate()));
	eventCamera.SetRotation(TEasing::InQuad(frontCamera->rotation, backCamera->rotation, moveTimer.GetTimeRate()));

	eventCamera.Update();
}

void EventCameraManager::Register(std::string string, std::vector<EventCamData> datas)
{
	eventCameraDatas[string] = datas;
}