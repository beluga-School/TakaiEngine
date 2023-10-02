#pragma once
#include "ViewProjection.h"
#include <list>
#include "Obj.h"

struct EventCamData
{
	Vector3 pos{};
	Vector3 rotation{};
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