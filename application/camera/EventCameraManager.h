#pragma once
#include <EventCamera.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <TEasing.h>
#include "ImguiManager.h"

//マネージャーで管理する用のデータ
struct EventCamManageData
{
	std::vector<EventCamData> datas;
	Vector3 target = { -1000,-1000,-1000 };

	//ターゲットデータに情報が入っているならtrueを返す
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
	/// イベント名を指定すると、それに対応したカメラを呼び出す
	/// </summary>
	/// <param name="eventname">検索したいイベント名</param>
	/// <returns>成功or失敗</returns>
	bool SetEventCamera(const std::string& eventname);

	bool GetEventEnd();

	void Start();

	void Update();

	void Register(std::string string, EventCamManageData datas,float movetime = 2.5f);

	void Reset();

	void DebugGUI();

	//イージング移動の始点カメラ情報
	EventCamData* frontCamera = nullptr;

	//イージング移動の終点カメラ情報
	EventCamData* backCamera = nullptr;

	//イベントカメラ情報配列をイベント名で保持するマップ
	std::unordered_map<std::string, EventCamManageData> eventCameraDatas;

	//イベントカメラ本体
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

