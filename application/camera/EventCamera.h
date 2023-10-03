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
	void SetPos(const Vector3& position);
	void SetRotation(const Vector3& rotation);

	void Initialize();
	void Update();
	void Draw();

	EventCamera() {};
	~EventCamera() {};

private:
	//�f�o�b�O�p�J�����ʒu�\���I�u�W�F�N�g
	Obj3d hontai;
	Obj3d target;

	float targetRadius = 10.0f;
};