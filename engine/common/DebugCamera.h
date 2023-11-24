#pragma once
#include <Obj.h>
#include <Camera.h>
#include "TEasing.h"

/*! DebugCamera
	デバッグカメラ用の3dオブジェクト 
	これの位置や回転を動かし、追従させる設計
*/
class DebugCamera : public Obj3d
{
public:
	//初期化
	void Initialize();
	//更新
	void Update();
	//視野を設定
	void SetRadius(float val);
	//視野を取得
	float GetRadius();
private:
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	Vector3 mCenterVec;
	Vector3 mSideVec;

	float mRadius = 20.0f;

	const float MOVE_SPEED = 1.0f;
};

