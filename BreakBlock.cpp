#include "BreakBlock.h"
#include "ObjParticle.h"
#include "Player.h"

void BreakBlock::Initialize()
{
	Obj3d::Initialize();
	SetTexture(TextureManager::GetTexture("white"));
}

void BreakBlock::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void BreakBlock::Draw()
{
	Obj3d::DrawMaterial();
}

void BreakBlock::HitEffect()
{
	for (int32_t i = 0; i < 10; i++)
	{
		ParticleManager::Get()->CreateCubeParticle(position, { 1,1,1 }, 5.f, { 28.f/255.f,64.f/255.f,42.f/255.f,1 });
	}

	scale = { 0,0,0 };

	isDead = true;
}
