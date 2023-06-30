#include "GEnemy.h"

void GEnemy::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("firewisp"));
	SetTexture(TextureManager::GetTexture("white"));
}

void GEnemy::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void GEnemy::Draw()
{
	Obj3d::DrawMaterial();
}
