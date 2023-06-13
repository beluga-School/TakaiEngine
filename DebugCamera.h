#pragma once
#include <Obj.h>
#include <ViewProjection.h>

//デバッグカメラ用の3dオブジェクト これの位置や回転を動かし、追従させる設計
class DebugCamera : public Obj3d
{
public:
	void Initialize();
	void Update();
private:
	float verticalRad = 0;
	float horizontalRad = 0;

	Vector3 centerVec;
	Vector3 sideVec;

	float radius = 20.0f;

	const float moveSpeed = 1.0f;
};

