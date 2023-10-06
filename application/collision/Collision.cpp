#include "Collision.h"
#include "Vector2.h"
#include "MathF.h"
#include <math.h>

bool Collsions::RayPlaneCollision(const Ray& ray, const Plane& plane)
{
	const float EPSILON = 1.0e-5f;

	float d1 = plane.normal.dot(ray.direction);
	//float d1 = ray.direction.dot(plane.normal);
	//レイが平面と交わらない方向にのびているなら当たらないのでfalse
	if (d1 > 0)
	{
		return false;
	}

	float d2 = plane.normal.dot(ray.start);
	//float d2 = ray.start.dot(plane.normal);

	float distance = d2 - plane.distance;

	float t = distance / -d1;

	if (t < 0)
	{
		return false;
	}

	return true;
}

Plane CreatePlane(Vector3 leftUp, Vector3 rightDown,Vector3 planeCenterPoint,  Vector3 planeNormal)
{
	//答え
	float c = abs(planeNormal.dot(planeCenterPoint));

	Plane plane;
	plane.distance = c;
	plane.normal = planeNormal;

	return plane;
}
//やろうと思ったけど一回諦めようかな....
RayCubeColOutput Collsions::RayCubeCollision(const Ray& ray, const Cube& cube)
{
	RayCubeColOutput output;

	//XY平面のZ座標手前の平面との当たり判定
	Vector3 leftUp = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	Vector3 rightDown = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	Vector3 planeCenterPoint = {
		(rightDown.x + leftUp.x) / 2,
		(rightDown.y + leftUp.y) / 2,
		leftUp.z
	};

	Plane frontXY = CreatePlane(leftUp, rightDown, planeCenterPoint,{ 0,0,-1 });

	
	if (Collsions::RayPlaneCollision(ray, frontXY))
	{
		output.XYCenter = true;
		output.none = false;
	}

	//XY平面のZ座標奥の平面との当たり判定
	leftUp = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	rightDown = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	planeCenterPoint = {
		(rightDown.x + leftUp.x) / 2,
		(rightDown.y + leftUp.y) / 2,
		leftUp.z
	};

	Plane backXY = CreatePlane(leftUp, rightDown, planeCenterPoint,{0,0,1});

	if (Collsions::RayPlaneCollision(ray, backXY))
	{
		output.XYback = true;
		output.none = false;
	}

	//XZ平面のY座標上の平面との当たり判定
	leftUp = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	rightDown = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	planeCenterPoint = {
		(rightDown.x + leftUp.x) / 2,
		leftUp.y,
		(rightDown.z + leftUp.z) / 2,
	};

	Plane upXZ = CreatePlane(leftUp, rightDown, planeCenterPoint,{ 0,1,0 });

	if (Collsions::RayPlaneCollision(ray, upXZ))
	{
		output.XZUp = true;
		output.none = false;
	}

	//XZ平面のY座標下の平面との当たり判定
	leftUp = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	rightDown = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	planeCenterPoint = {
		(rightDown.x + leftUp.x) / 2,
		leftUp.y,
		(rightDown.z + leftUp.z) / 2,
	};

	Plane downXZ = CreatePlane(leftUp, rightDown, planeCenterPoint,{ 0,-1,0 });

	if (Collsions::RayPlaneCollision(ray, downXZ))
	{
		output.XZDown = true;
		output.none = false;
	}

	//YZ平面のX座標左の平面との当たり判定
	leftUp = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	rightDown = {
		cube.position.x - cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	planeCenterPoint = {
		leftUp.x,
		(rightDown.y + leftUp.y) / 2,
		(rightDown.z + leftUp.z) / 2,
	};

	Plane leftYZ = CreatePlane(leftUp, rightDown, planeCenterPoint,{ -1,0,0 });

	if (Collsions::RayPlaneCollision(ray, leftYZ))
	{
		output.YZLeft = true;
		output.none = false;
	}

	//YZ平面のX座標右の平面との当たり判定
	leftUp = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y + cube.scale.y / 2,
		cube.position.z + cube.scale.z / 2,
	};

	rightDown = {
		cube.position.x + cube.scale.x / 2,
		cube.position.y - cube.scale.y / 2,
		cube.position.z - cube.scale.z / 2,
	};

	//平面の中心点(bベクトル)
	planeCenterPoint = {
		leftUp.x,
		(rightDown.y + leftUp.y) / 2,
		(rightDown.z + leftUp.z) / 2,
	};

	Plane rightYZ = CreatePlane(leftUp, rightDown, planeCenterPoint,{ 1,0,0 });

	if (Collsions::RayPlaneCollision(ray, rightYZ))
	{
		output.YZRight = true;
		output.none = false;
	}

	return output;
}

bool Collsions::RectangleXZCollision(const Cube& cube1, const Cube& cube2)
{
	float DistanceX = cube1.position.x - cube2.position.x;
	float DistanceZ = cube1.position.z - cube2.position.z;

	DistanceX = MathF::Abs(DistanceX);
	DistanceZ = MathF::Abs(DistanceZ);

	if (DistanceX <= cube1.scale.x * 0.5f + cube2.scale.x * 0.5f &&
		DistanceZ <= cube1.scale.z * 0.5f + cube2.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}

Vector3 RayPlaneInter(const Ray& ray, const Plane& plane)
{
	Vector3 inter = {0,0,0};

	float d1 = plane.normal.dot(ray.direction);
	//float d1 = ray.direction.dot(plane.normal);
	//レイが平面と交わらない方向にのびているなら当たらないのでfalse
	if (d1 > 0)
	{
		return {0,0,0};
	}

	float d2 = plane.normal.dot(ray.start);
	//float d2 = ray.start.dot(plane.normal);

	float distance = d2 - plane.distance;

	float t = distance / -d1;

	if (t < 0)
	{
		return { 0,0,0 };
	}

	inter = ray.start + (t * ray.direction);
	return inter;
}

bool Collsions::CubeCollision(const Cube& cube1, const Cube& cube2)
{
	float DistanceX = cube1.position.x - cube2.position.x;
	float DistanceY = cube1.position.y - cube2.position.y;
	float DistanceZ = cube1.position.z - cube2.position.z;

	DistanceX = MathF::Abs(DistanceX);
	DistanceY = MathF::Abs(DistanceY);
	DistanceZ = MathF::Abs(DistanceZ);

	if (DistanceX <= cube1.scale.x * 0.5f + cube2.scale.x * 0.5f &&
		DistanceY <= cube1.scale.y * 0.5f + cube2.scale.y * 0.5f &&
		DistanceZ <= cube1.scale.z * 0.5f + cube2.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}

bool Collsions::BoxColAABB(const Obj3d& colA, const Obj3d& colB)
{
	int32_t DistanceX = static_cast<int32_t>(colA.matWorld.m[3][0] - colB.matWorld.m[3][0]);
	int32_t DistanceY = static_cast<int32_t>(colA.matWorld.m[3][1] - colB.matWorld.m[3][1]);
	int32_t DistanceZ = static_cast<int32_t>(colA.matWorld.m[3][2] - colB.matWorld.m[3][2]);

	DistanceX = MathF::Abs(DistanceX);
	DistanceY = MathF::Abs(DistanceY);
	DistanceZ = MathF::Abs(DistanceZ);

	if (DistanceX <= colA.scale.x * 0.5f + colB.scale.x * 0.5f &&
		DistanceY <= colA.scale.y * 0.5f + colB.scale.y * 0.5f &&
		DistanceZ <= colA.scale.z * 0.5f + colB.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}

bool Collsions::SpherePlaneCollision(const Sphere& sphere, const Plane& plane)
{
	float distV = sphere.center.dot(plane.normal);
	float dist = distV - plane.distance;

	if (fabsf(dist) > sphere.radius)return false;

	return true;
}

Vector3 Collsions::ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle)
{
	//pointがp0の外側にあるかをチェック
	Vector3 p0_p1 = triangle.pos1 - triangle.pos0;
	Vector3 p0_p2 = triangle.pos2 - triangle.pos0;
	Vector3 p0_pt = point - triangle.pos0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		return triangle.pos0;
	}

	//pointがp1の外側にあるかをチェック
	Vector3 p1_pt = point - triangle.pos1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= 0.0f)
	{
		return triangle.pos1;
	}

	// pointがp0_p1の辺領域の中にあるかどうかチェックし、あればpointのp0_p1上に対する射影を返す
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return triangle.pos0 + v * p0_p1;
	}

	// pointがp2の外側の頂点領域の中にあるかどうかチェック
	Vector3 p2_pt = point - triangle.pos2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return triangle.pos2;
	}

	// pointがp0_p2の辺領域の中にあるかどうかチェックし、あればpointのp0_p2上に対する射影を返す
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return triangle.pos0 + w * p0_p2;
	}

	// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return triangle.pos1 + w * (triangle.pos2 - triangle.pos1);
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	return triangle.pos0 + p0_p1 * v + p0_p2 * w;
}

bool Collsions::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle)
{
	Vector3 p = Collsions::ClosestPtPoint2Triangle(sphere.center, triangle);
	//点pと級の中心の差分ベクトル
	Vector3 v = p - sphere.center;
	//距離の二乗を求める
	float vDot = v.dot(v);
	//球と三角形の距離が半径以下なら当たっていない
	if (vDot > sphere.radius * sphere.radius)
	{
		return false;
	}

	return true;
}

bool Collsions::SphereCollsion(const Sphere& sphere1, const Sphere& sphere2)
{
	return	(sphere2.center.x - sphere1.center.x) * (sphere2.center.x - sphere1.center.x) +
			(sphere2.center.y - sphere1.center.y) * (sphere2.center.y - sphere1.center.y) +
			(sphere2.center.z - sphere1.center.z) * (sphere2.center.z - sphere1.center.z) <=
			(sphere1.radius + sphere2.radius) * (sphere1.radius + sphere2.radius);
}

bool Cube::operator==(const Cube& cube)const
{
	return (cube.position.x == position.x &&
		cube.position.y == position.y &&
		cube.position.z == position.z &&
		cube.scale.x == scale.x &&
		cube.scale.y == scale.y &&
		cube.scale.z == scale.z);
}

Sphere::Sphere()
{
	center = { 0,0,0 };
	radius = 1.0f;
}

Sphere::Sphere(const Vector3& center_, float radius_)
{
	center = center_;
	radius = radius_;
}
