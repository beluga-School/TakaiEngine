#pragma once
#include "ViewProjection.h"
#include <list>
#include "Obj.h"

struct EventCamData
{
	Vector3 pos{};
	Vector3 rotation{};

	std::string eventnumber = "";
};

class EventCamera
{
public:
	void Initialize();
	void Update();
	void Draw();

	void ObjUpdate();

	static EventCamera* Get() {
		static EventCamera instance;
		return &instance;
	}

	std::list<EventCamData> eventCamDatas;

private:
	EventCamera(){};
	~EventCamera(){};

	Obj3d hontai;
	Obj3d target;

	Vector3 hoge;

};

