#include "Collision.h"
#include "Vector2.h"
#include "MathF.h"
#include <math.h>

bool Collsions::RayPlaneCollision(const Ray& ray, const Plane& plane)
{
	const float EPSILON = 1.0e-5f;

	float d1 = plane.normal.dot(ray.direction);
	//float d1 = ray.direction.dot(plane.normal);
	//���C�����ʂƌ����Ȃ������ɂ̂тĂ���Ȃ瓖����Ȃ��̂�false
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
	//����
	float c = abs(planeNormal.dot(planeCenterPoint));

	Plane plane;
	plane.distance = c;
	plane.normal = planeNormal;

	return plane;
}
//��낤�Ǝv�������ǈ����߂悤����....
RayCubeColOutput Collsions::RayCubeCollision(const Ray& ray, const Cube& cube)
{
	RayCubeColOutput output;

	//XY���ʂ�Z���W��O�̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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

	//XY���ʂ�Z���W���̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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

	//XZ���ʂ�Y���W��̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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

	//XZ���ʂ�Y���W���̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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

	//YZ���ʂ�X���W���̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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

	//YZ���ʂ�X���W�E�̕��ʂƂ̓����蔻��
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

	//���ʂ̒��S�_(b�x�N�g��)
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
	//���C�����ʂƌ����Ȃ������ɂ̂тĂ���Ȃ瓖����Ȃ��̂�false
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
	//point��p0�̊O���ɂ��邩���`�F�b�N
	Vector3 p0_p1 = triangle.pos1 - triangle.pos0;
	Vector3 p0_p2 = triangle.pos2 - triangle.pos0;
	Vector3 p0_pt = point - triangle.pos0;

	float d1 = p0_p1.dot(p0_pt);
	float d2 = p0_p2.dot(p0_pt);

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		return triangle.pos0;
	}

	//point��p1�̊O���ɂ��邩���`�F�b�N
	Vector3 p1_pt = point - triangle.pos1;

	float d3 = p0_p1.dot(p1_pt);
	float d4 = p0_p2.dot(p1_pt);

	if (d3 >= 0.0f && d4 <= 0.0f)
	{
		return triangle.pos1;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		return triangle.pos0 + v * p0_p1;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	Vector3 p2_pt = point - triangle.pos2;

	float d5 = p0_p1.dot(p2_pt);
	float d6 = p0_p2.dot(p2_pt);
	if (d6 >= 0.0f && d5 <= d6)
	{
		return triangle.pos2;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		return triangle.pos0 + w * p0_p2;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
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
	//�_p�Ƌ��̒��S�̍����x�N�g��
	Vector3 v = p - sphere.center;
	//�����̓������߂�
	float vDot = v.dot(v);
	//���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
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
