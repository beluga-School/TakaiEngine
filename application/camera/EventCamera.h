#pragma once
#include "Camera.h"
#include <list>
#include "Obj.h"

struct EventCamData
{
	Vector3 pos{};
	Vector3 rotation{};
};

/*! EventCamera
	イベント時に使用するカメラの情報を3Dオブジェクトに保存して管理するクラス
*/
class EventCamera
{
public:
	void SetPos(const Vector3& position);
	void SetTarget(const Vector3& targetpos);
	void SetRotation(const Vector3& rotation);

	void Initialize();
	void Update();
	void Draw();

	EventCamera() {};
	~EventCamera() {};

private:
	//デバッグ用カメラ位置表示オブジェクト
	Obj3d hontai;
	Obj3d target;

	float targetRadius = 10.0f;

	bool useTarget = false;
};