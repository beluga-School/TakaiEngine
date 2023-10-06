#pragma once
#include <EventCamera.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <TEasing.h>
#include "ImguiManager.h"

//�}�l�[�W���[�ŊǗ�����p�̃f�[�^
struct EventCamManageData
{
	std::vector<EventCamData> datas;
	Vector3 target = { -1000,-1000,-1000 };

	//�^�[�Q�b�g�f�[�^�ɏ�񂪓����Ă���Ȃ�true��Ԃ�
	bool InTargetData();
};

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

	bool GetEventEnd();

	void Start();

	void Update();

	void Register(std::string string, EventCamManageData datas,float movetime = 2.5f);

	void Reset();

	void DebugGUI();

	//�C�[�W���O�ړ��̎n�_�J�������
	EventCamData* frontCamera = nullptr;

	//�C�[�W���O�ړ��̏I�_�J�������
	EventCamData* backCamera = nullptr;

	//�C�x���g�J�������z����C�x���g���ŕێ�����}�b�v
	std::unordered_map<std::string, EventCamManageData> eventCameraDatas;

	//�C�x���g�J�����{��
	EventCamera eventCamera{};

private:
	EventCameraManager() {};
	~EventCameraManager() {};

	TEasing::easeTimer moveTimer = 5.0f;
	TEasing::easeTimer rotaTimer = 1.0f;

	GUI hoge = { "hogeDebug" };

	std::string eventname_ = "";

	std::vector<Vector3> eventPositions;

	int32_t itrNumber = -1;
};

