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
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	Vector3 mCenterVec;
	Vector3 mSideVec;

	float mRadius = 20.0f;

	const float mMoveSpeed = 1.0f;
};

