#include "Bullet.h"
#include "ViewProjection.h"
#include "TimeManager.h"

void Bullet::Initialize(const Vector3& position_)
{
	position = position_;
	MODEL = ModelManager::Get()->GetModel("Cube");
	TEXTURE = TextureManager::Get()->GetTexture("white");
	scale = { 5,5,5 };
	color_ = { 1.0f,1.0f,0.0f,1.0f };
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

	Obj3d::Update(*Camera::sCamera);
	cubeCol.position = position;
	cubeCol.scale = scale;
}

void Bullet::Draw()
{
	Obj3d::Draw();
}
