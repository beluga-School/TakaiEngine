#pragma once
#include <Obj.h>
#include <ViewProjection.h>

//�f�o�b�O�J�����p��3d�I�u�W�F�N�g ����̈ʒu���]�𓮂����A�Ǐ]������݌v
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

