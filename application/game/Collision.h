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
	float distance = 0.0f;
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

struct Sphere
{
	Vector3 center = { 0,0,0 };
	float radius = 1.0f;
};

struct Triangle
{
	Vector3 pos0;
	Vector3 pos1;
	Vector3 pos2;

	Vector3 normal;
};

namespace Collsions
{
	//���C�ƕ��ʂ̓����蔻��
	bool RayPlaneCollision(const Ray& ray, const Plane& plane);

	//y����0�Ƃ��āAx��z��2���Ŏl�p���m�Ŕ��肷�铖���蔻��
	bool RectangleXZCollision(const Cube& cube1, const Cube& cube2);

	bool CubeCollision(const Cube& cube1, const Cube& cube2);

	bool BoxColAABB(const Obj3d& colA, const Obj3d& colB);

	bool SpherePlaneCollision(const Sphere& sphere, const Plane& plane);

	Vector3 ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle);

	bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle);
}