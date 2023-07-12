#include "Star.h"

void Star::Initialize()
{
	SetModel(ModelManager::GetModel("star"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Star::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Star::Draw()
{
	Obj3d::DrawMaterial();
}

void Star::HitEffect()
{

}
