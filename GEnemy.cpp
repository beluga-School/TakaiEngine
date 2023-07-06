#include "GEnemy.h"

void GEnemy::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("firewisp"));
	SetTexture(TextureManager::GetTexture("white"));

	hitSphere.Initialize();
	hitSphere.SetModel(ModelManager::GetModel("BlankSphere"));
	hitSphere.SetTexture(TextureManager::GetTexture("white"));

	sphereCol.center = position;
	sphereCol.radius = 15;

	hitSphere.position = sphereCol.center;
	hitSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };
}

void GEnemy::Update()
{
	sphereCol.center = position;

	hitSphere.position = sphereCol.center;
	hitSphere.scale = { sphereCol.radius,sphereCol.radius,sphereCol.radius };

	Obj3d::Update(*Camera::sCamera);

	hitSphere.Update(*Camera::sCamera);
}

void GEnemy::Draw()
{
	Obj3d::DrawMaterial();

	hitSphere.Draw();
}

void GEnemy::HitEffect()
{
	isDead = true;
}
