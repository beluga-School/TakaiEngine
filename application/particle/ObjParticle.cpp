#include "ObjParticle.h"
#include "MathF.h"
#include "TimeManager.h"
#include "Camera.h"
#include "ClearDrawScreen.h"

using namespace std;

ParticleManager* ParticleManager::Get()
{
    static ParticleManager instance;
    return &instance;
}

void ParticleManager::CreatePool()
{
	for (int32_t i = 0; i < maxPool; i++)
	{
		cubePool.emplace_back(std::make_unique<CubeParticle>());
	}
}

void ParticleManager::AllDelete()
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		pat->isdead = true;
	}
}

void ParticleManager::CreateCubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color)
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (pat->isdead) {
			pat->Set(pos, scale, moveDistance,color);
			return;
		}
	}

	
	cubePool.emplace_back(std::make_unique<CubeParticle>(pos, scale, moveDistance, color));
}

void ParticleManager::Update()
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->Update();
		}
	}
}

void ParticleManager::Draw()
{
	BasicObjectPreDraw("OutLine",false);
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->cube.DrawOutLine();
		}
	}
	
	BasicObjectPreDraw("Toon");
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->Draw();
		}
	}
}

CubeParticle::CubeParticle()
{
	cube.Initialize();
	cube.SetTexture(TextureManager::Get()->GetTexture("white"));

	cube.color_ = {
		218.0f / 255.0f,
		159.0f / 255.0f,
		64.0f / 255.0f,
		1 };

	cube.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

CubeParticle::CubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color)
{
	cube.Initialize();
	cube.SetTexture(TextureManager::Get()->GetTexture("white"));

	Set(pos,scale, moveDistance, color);

	cube.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

void CubeParticle::Set(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color)
{
	cube.position ={
		pos.x,
		pos.y,
		pos.z
	};

	cube.scale = {
		scale.x,
		scale.y,
		scale.z
	};

	cube.color_ = {
		color.x,
		color.y,
		color.z,
		color.w };

	moveSpeed.x = MathF::GetRand(-1.0f,1.0f) * moveDistance;
	moveSpeed.y = MathF::GetRand(-1.0f,1.0f) * moveDistance;
	moveSpeed.z = MathF::GetRand(-1.0f,1.0f) * moveDistance;

	rotateSpeed = {
		MathF::GetRand(-1.0f, 1.0f) * rotateMag,
		MathF::GetRand(-1.0f, 1.0f) * rotateMag,
		MathF::GetRand(-1.0f, 1.0f) * rotateMag
	};
	
	shrinkSpeed = {
		scale.x,
		scale.y,
		scale.z
	};

	isdead = false;
}

void CubeParticle::Update()
{
	float minScale = 0.1f;
	if (cube.scale.x <= minScale ||
		cube.scale.y <= minScale ||
		cube.scale.z <= minScale
		)
	{
		isdead = true;
	}

	cube.position.x += moveSpeed.x * TimeManager::deltaTime;
	cube.position.y += moveSpeed.y * TimeManager::deltaTime;
	cube.position.z += moveSpeed.z * TimeManager::deltaTime;

	cube.scale.x -= shrinkSpeed.x * TimeManager::deltaTime;
	cube.scale.y -= shrinkSpeed.y * TimeManager::deltaTime;
	cube.scale.z -= shrinkSpeed.z * TimeManager::deltaTime;

	cube.rotation.x += rotateSpeed.x * TimeManager::deltaTime;
	cube.rotation.y += rotateSpeed.y * TimeManager::deltaTime;
	cube.rotation.z += rotateSpeed.z * TimeManager::deltaTime;

	cube.Update(*Camera::sCamera);
}
