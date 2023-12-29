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
	Vector3 rotation;
	
	bool operator==(const Cube& cube)const;
};

struct Sphere
{
	Sphere();
	Sphere(const Vector3& center,float radius);

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

struct RayCubeColOutput
{
	bool XYCenter = false;
	bool XYback = false;
	bool XZUp = false;
	bool XZDown = false;
	bool YZRight = false;
	bool YZLeft = false;
	//どれとも当たってない
	bool none = true;
};

/*! Collsions
	オブジェクト同士の衝突判定関数のまとめ
*/
namespace Collsions
{
	//レイと平面の当たり判定
	bool RayPlaneCollision(const Ray& ray, const Plane& plane);

	//レイとキューブの当たり判定
	RayCubeColOutput RayCubeCollision(const Ray& ray, const Cube& cube);

	//y軸を0として、xとzの2軸で四角同士で判定する当たり判定
	bool RectangleXZCollision(const Cube& cube1, const Cube& cube2);

	//キューブの当たり判定
	bool CubeCollision(const Cube& cube1, const Cube& cube2);

	//AABBの当たり判定
	bool BoxColAABB(const Obj3d& colA, const Obj3d& colB);

	//球と平面の当たり判定
	bool SpherePlaneCollision(const Sphere& sphere, const Plane& plane);

	//点と三角形の当たり判定
	Vector3 ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle);

	//球と三角形の当たり判定
	bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle);

	//球同士の当たり判定
	bool SphereCollsion(const Sphere& sphere1, const Sphere& sphere2);
}