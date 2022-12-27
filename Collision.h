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
	float distance;
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


//レイと平面の当たり判定
bool RayPlaneCollision(Ray ray,Plane plane);

//y軸を0として、xとzの2軸で四角同士で判定する当たり判定
bool RectangleXZCollision(Cube cube1, Cube cube2);

bool CubeCollision(Cube cube1, Cube cube2);
bool PlaneCubeCollision(Plane plane, Cube cube);

bool BoxColAABB(const Obj3d& colA, const Obj3d& colB);