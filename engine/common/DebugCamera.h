#pragma once
#include <Obj.h>
#include <ViewProjection.h>
#include "TEasing.h"

//�f�o�b�O�J�����p��3d�I�u�W�F�N�g ����̈ʒu���]�𓮂����A�Ǐ]������݌v
class DebugCamera : public Obj3d
{
public:
	void Initialize();
	void Update();

	void SetRadius(float val);
	float GetRadius();
private:
	float mVerticalRad = 0;
	float mHorizontalRad = 0;

	Vector3 mCenterVec;
	Vector3 mSideVec;

	float mRadius = 20.0f;

	const float MOVE_SPEED = 1.0f;
};

