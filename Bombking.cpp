#include "Bombking.h"

void Bombking::Initialize()
{
	Obj3d::Initialize();
	SetModel(ModelManager::GetModel("bombking"));
	SetTexture(TextureManager::GetTexture("white"));
	SetOutLineState({ 0,0,0,1 }, 0.05f);
}

void Bombking::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Bombking::Draw()
{
	Obj3d::DrawMaterial();
}

void Bombking::HitEffect()
{
}

void Bombking::Encount()
{
}
