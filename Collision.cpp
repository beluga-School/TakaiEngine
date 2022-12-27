#include "Collision.h"
#include "Vector2.h"

template <typename T>
T Abs(T a)
{
	if (a < 0)
	{
		a = -a;
	}
	return a;
}

bool RayPlaneCollision(Ray ray, Plane plane)
{
	const float epsilon = 1.0e-5f;

	float d1 = plane.normal.dot(ray.direction);
	//float d1 = ray.direction.dot(plane.normal);
	//ƒŒƒC‚ª•½–Ê‚ÆŒð‚í‚ç‚È‚¢•ûŒü‚É‚Ì‚Ñ‚Ä‚¢‚é‚È‚ç“–‚½‚ç‚È‚¢‚Ì‚Åfalse
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

bool RectangleXZCollision(Cube cube1, Cube cube2)
{
	float DistanceX = cube1.position.x - cube2.position.x;
	float DistanceZ = cube1.position.z - cube2.position.z;

	DistanceX = Abs(DistanceX);
	DistanceZ = Abs(DistanceZ);

	if (DistanceX <= cube1.scale.x * 0.5f + cube2.scale.x * 0.5f &&
		DistanceZ <= cube1.scale.z * 0.5f + cube2.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}

Vector3 RayPlaneInter(Ray ray, Plane plane)
{
	Vector3 inter = {0,0,0};

	float d1 = plane.normal.dot(ray.direction);
	//float d1 = ray.direction.dot(plane.normal);
	//ƒŒƒC‚ª•½–Ê‚ÆŒð‚í‚ç‚È‚¢•ûŒü‚É‚Ì‚Ñ‚Ä‚¢‚é‚È‚ç“–‚½‚ç‚È‚¢‚Ì‚Åfalse
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

bool CubeCollision(Cube cube1, Cube cube2)
{
	float DistanceX = cube1.position.x - cube2.position.x;
	float DistanceY = cube1.position.y - cube2.position.y;
	float DistanceZ = cube1.position.z - cube2.position.z;

	DistanceX = Abs(DistanceX);
	DistanceY = Abs(DistanceY);
	DistanceZ = Abs(DistanceZ);

	if (DistanceX <= cube1.scale.x * 0.5f + cube2.scale.x * 0.5f &&
		DistanceY <= cube1.scale.y * 0.5f + cube2.scale.y * 0.5f &&
		DistanceZ <= cube1.scale.z * 0.5f + cube2.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}

bool BoxColAABB(const Obj3d& colA, const Obj3d& colB)
{
	int DistanceX = colA.matWorld.m[3][0] - colB.matWorld.m[3][0];
	int DistanceY = colA.matWorld.m[3][1] - colB.matWorld.m[3][1];
	int DistanceZ = colA.matWorld.m[3][2] - colB.matWorld.m[3][2];

	DistanceX = Abs(DistanceX);
	DistanceY = Abs(DistanceY);
	DistanceZ = Abs(DistanceZ);

	if (DistanceX <= colA.scale.x * 0.5f + colB.scale.x * 0.5f &&
		DistanceY <= colA.scale.y * 0.5f + colB.scale.y * 0.5f &&
		DistanceZ <= colA.scale.z * 0.5f + colB.scale.z * 0.5f)
	{
		return true;
	}
	return false;
}