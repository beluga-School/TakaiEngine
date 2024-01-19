#include "Grass.h"
#include "MathF.h"
#include "ObjParticle.h"

void Grass::LoadResource()
{
	TextureManager::Load("Resources\\leaf.png", "leaf");
}

void Grass::Initialize()
{
	SetModel(ModelManager::GetModel("teiboku"));
	SetTexture(TextureManager::GetTexture("default"));
}

void Grass::Update()
{
	minScale = saveScale * 0.8f;
	timer.Update();

	if (timer.GetNowEnd())
	{
		bounceCount++;
	}
	if (bounceCount % 2 == 0)
	{
		if (timer.GetNowEnd())
		{
			Vector3 startPos = position + Vector3(0,2,0);
			Vector3 endPos = Util::GetRandVector3(startPos,-2.0f,2.0f);
			
			float partSize = 1.f;

			ParticleManager::Get()->CreateTextureParticle(
				startPos,
				endPos,
				{ partSize ,partSize },
				1.0f,
				"leaf",
				EASEPATTERN::INQUAD);

			if (bounceCount > 5)
			{
				for (int32_t i = 0; i < 3; i++)
				{
					ParticleManager::Get()->CreateTextureParticle(
						startPos,
						endPos,
						{ partSize ,partSize },
						1.0f,
						"leaf",
						EASEPATTERN::INQUAD);
				}
			}
		}
	}
	if (bounceCount > 5)
	{
		rotation.x = TEasing::InQuad(0, MathF::AngleConvRad(20), timer.GetTimeRate());
		scale.y = TEasing::InQuad(minScale.y, saveScale.y, timer.GetTimeRate());
		if (timer.GetReverseEnd())
		{
			bounceCount = 0;
		}
	}
	else
	{
		scale.y = TEasing::InQuad(minScale.y, saveScale.y,timer.GetTimeRate());
	}

	timer.Roop();

	Obj3d::Update(*Camera::sCamera);
}

void Grass::Draw()
{
	Obj3d::DrawMaterial();
}
