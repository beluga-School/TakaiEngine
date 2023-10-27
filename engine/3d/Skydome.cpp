#include "Skydome.h"
#include "Camera.h"

void Skydome::Initialize()
{
	SetModel(ModelManager::GetModel("skydome"));
	SetTexture(TextureManager::Get()->GetTexture("white"));
	scale = { 10,10,10 };
	color_ = { 1.f,1.f,1.f,1.0f };
}

void Skydome::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Skydome::Draw()
{
	Obj3d::DrawMaterial();
}
