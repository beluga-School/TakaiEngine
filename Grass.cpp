#include "Grass.h"
#include "MathF.h"
#include "ObjParticle.h"

void Grass::LoadResource()
{
	//TextureManager::Load("Resources\\movegrass.png", "movegrass");
}

void Grass::Initialize()
{
	SetModel(ModelManager::GetModel("grass"));
	SetTexture(TextureManager::GetTexture("default"));
}

void Grass::Update()
{
	timer.Update();

	if (timer.GetNowEnd())
	{
		bounceCount++;
	}
	if (bounceCount % 2 == 0)
	{
		if (timer.GetNowEnd())
		{
			Vector3 partPos = position;
			partPos.y += 1.0f;
			float partSize = 1.0f;

			ParticleManager::Get()->CreateCubeParticle(partPos, { partSize,partSize,partSize }, 10.f, { 0,1,0,1 }, "", PARTICLEPATTERN::DROP);
			if (bounceCount > 5)
			{
				for (int32_t i = 0; i < 3; i++)
				{
					ParticleManager::Get()->CreateCubeParticle(partPos, { partSize,partSize,partSize }, 10.f, { 0,1,0,1 }, "", PARTICLEPATTERN::DROP);
				}
			}
		}
	}
	if (bounceCount > 5)
	{
		rotation.x = TEasing::InQuad(0, MathF::AngleConvRad(20), timer.GetTimeRate());
		scale.y = TEasing::InQuad(0.8f, 1.0f, timer.GetTimeRate());
		if (timer.GetReverseEnd())
		{
			bounceCount = 0;
		}
	}
	else
	{
		scale.y = TEasing::InQuad(0.8f, 1.0f,timer.GetTimeRate());
	}

	timer.Roop();

	Obj3d::Update(*Camera::sCamera);
}

void Grass::Draw()
{
	Obj3d::DrawMaterial();
}
