#pragma once
#include "ViewProjection.h"
#include <list>
#include "Obj.h"

struct EventCamData
{
	Vector3 pos{};
	Vector3 rotation{};

	std::string eventName = "";
};

class EventCamera
{
public:
	void Initialize();
	void Update();
	void Draw();

	//読み込んだカメラデータ
	EventCamData eventCamData;

	EventCamera() {};
	~EventCamera() {};

private:
	//デバッグ用カメラ位置表示オブジェクト
	Obj3d hontai;
	Obj3d target;

	float targetRadius = 10.0f;
};

class EventCameraManager
{
public:
	void Update();
	void Draw();

	void Register(const EventCamData &eventCamData);
	void Clear();

	static EventCameraManager* Get()
	{
		static EventCameraManager instance;
		return &instance;
	}

private:
	EventCameraManager(){};
	~EventCameraManager(){};

	std::list<EventCamera> eventCameras;
};