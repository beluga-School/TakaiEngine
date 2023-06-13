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
	float verticalRad = 0;
	float horizontalRad = 0;

	Vector3 centerVec;
	Vector3 sideVec;

	float radius = 20.0f;

	const float moveSpeed = 1.0f;
};

