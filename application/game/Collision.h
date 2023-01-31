#pragma once
#include "Vector3.h"
#include "Obj.h"

struct Ray
{
public:
	//�n�_
	Vector3 start;
	
	//����
	Vector3 direction;
};

struct Plane
{
	//�@���x�N�g��
	Vector3 normal;
	float distance;
};

struct Cube
{
	Vector3 position;
	Vector3 scale;

	//���(������̖@���x�N�g��)
	Plane upPlane;

	Cube() {
		upPlane.normal = { 0, 1, 0 };
		upPlane.distance = 1.0f;
	};
};


//���C�ƕ��ʂ̓����蔻��
bool RayPlaneCollision(Ray ray,Plane plane);

//y����0�Ƃ��āAx��z��2���Ŏl�p���m�Ŕ��肷�铖���蔻��
bool RectangleXZCollision(Cube cube1, Cube cube2);

bool CubeCollision(Cube cube1, Cube cube2);
bool PlaneCubeCollision(Plane plane, Cube cube);

bool BoxColAABB(const Obj3d& colA, const Obj3d& colB);