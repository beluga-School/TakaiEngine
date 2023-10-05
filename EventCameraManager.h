#pragma once
#include <EventCamera.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <TEasing.h>
#include "ImguiManager.h"


class EventCameraManager
{
public:
	static EventCameraManager* Get() {
		static EventCameraManager instance;
		return &instance;
	};

	/// <summary>
	/// �C�x���g�����w�肷��ƁA����ɑΉ������J�������Ăяo��
	/// </summary>
	/// <param name="eventname">�����������C�x���g��</param>
	/// <returns>����or���s</returns>
	bool SetEventCamera(const std::string& eventname);

	void Start();

	void Update();

	void Register(std::string string, std::vector<EventCamData> datas);

	void Reset();

	void DebugGUI();

	//�C�[�W���O�ړ��̎n�_�J�������
	EventCamData* frontCamera = nullptr;

	//�C�[�W���O�ړ��̏I�_�J�������
	EventCamData* backCamera = nullptr;

	//�C�x���g�J�������z����C�x���g���ŕێ�����}�b�v
	std::unordered_map<std::string, std::vector<EventCamData>> eventCameraDatas;

	//�C�x���g�J�����{��
	EventCamera eventCamera{};


private:
	EventCameraManager() {};
	~EventCameraManager() {};

	TEasing::easeTimer moveTimer = 1.0f;

	GUI hoge = { "hogeDebug" };
};

