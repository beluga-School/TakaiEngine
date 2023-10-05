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
	/// イベント名を指定すると、それに対応したカメラを呼び出す
	/// </summary>
	/// <param name="eventname">検索したいイベント名</param>
	/// <returns>成功or失敗</returns>
	bool SetEventCamera(const std::string& eventname);

	void Start();

	void Update();

	void Register(std::string string, std::vector<EventCamData> datas);

	void Reset();

	void DebugGUI();

	//イージング移動の始点カメラ情報
	EventCamData* frontCamera = nullptr;

	//イージング移動の終点カメラ情報
	EventCamData* backCamera = nullptr;

	//イベントカメラ情報配列をイベント名で保持するマップ
	std::unordered_map<std::string, std::vector<EventCamData>> eventCameraDatas;

	//イベントカメラ本体
	EventCamera eventCamera{};


private:
	EventCameraManager() {};
	~EventCameraManager() {};

	TEasing::easeTimer moveTimer = 1.0f;

	GUI hoge = { "hogeDebug" };
};

