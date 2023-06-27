#pragma once
#include "Vector3.h"
#include "Obj.h"

struct Ray
{
public:
	//始点
	Vector3 start;
	
	//方向
	Vector3 direction;
};

struct Plane
{
	//法線ベクトル
	Vector3 normal;
	float distance = 0.0f;
};

struct Cube
{
	Vector3 position;
	Vector3 scale;

	//上面(上向きの法線ベクトル)
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
	//レイと平面の当たり判定
	bool RayPlaneCollision(const Ray& ray, const Plane& plane);

	//y軸を0として、xとzの2軸で四角同士で判定する当たり判定
	bool RectangleXZCollision(const Cube& cube1, const Cube& cube2);

	bool CubeCollision(const Cube& cube1, const Cube& cube2);

	bool BoxColAABB(const Obj3d& colA, const Obj3d& colB);

	bool SpherePlaneCollision(const Sphere& sphere, const Plane& plane);

	Vector3 ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle);

	bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle);
}