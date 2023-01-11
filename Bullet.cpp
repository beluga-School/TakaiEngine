#include "Bullet.h"
#include "ViewProjection.h"
#include "TimeManager.h"

void Bullet::Initialize(Vector3 position)
{
	this->position = position;
	model = &ModelManager::GetInstance()->cubeM;
	texture = &TextureManager::GetInstance()->white;
	scale = { 5,5,5 };
	color = { 1.0f,1.0f,0.0f,1.0f };
}

void Bullet::SetTarget(const Mob &targetMob)
{
	targetVec = targetMob.position - position;
	targetVec.normalize();
}

void Bullet::Update()
{
	lifeTimer -= TimeManager::deltaTime;
	if (lifeTimer < 0)isDead = true;

	//À•W‚ÌXV
	position += 150 * targetVec * TimeManager::deltaTime;

	Obj3d::Update(Camera::camera->matView,Camera::camera->matProjection);
	cubeCol.position = position;
	cubeCol.scale = scale;
}

void Bullet::Draw()
{
	Obj3d::Draw();
}
