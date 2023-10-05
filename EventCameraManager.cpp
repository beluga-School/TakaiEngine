#include "EventCameraManager.h"
#include "ViewProjection.h"
#include <EventManager.h>

bool EventCameraManager::SetEventCamera(const std::string& eventname)
{
	//�C�x���g��������Ȃ������玸�s�ŕԂ�
	if (eventCameraDatas.find(eventname) == eventCameraDatas.end()) return false;

	//�J��������������Ȃ�
	if (eventCameraDatas[eventname].size() > 1)
	{
		//�ԍ����Ⴂ����Z�b�g����
		frontCamera = &eventCameraDatas[eventname].front();
		backCamera = &eventCameraDatas[eventname].front() + 1;
	}
	//�����łȂ��Ȃ�
	else
	{
		//��O�ƌ��ɓ������̂��Z�b�g���Ă���
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

	DebugGUI();
}

void EventCameraManager::Register(std::string string, std::vector<EventCamData> datas)
{
	eventCameraDatas[string] = datas;
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
}
