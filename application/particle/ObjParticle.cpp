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
		spherePool.emplace_back(std::make_unique<SphereParticle>());
	}
}

void ParticleManager::AllDelete()
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		pat->isdead = true;
	}
	for (unique_ptr<SphereParticle>& pat : spherePool) {
		pat->isdead = true;
	}
}

void ParticleManager::CreateCubeParticle(const Vector3& pos, const Vector3& scale,
	const float& moveDistance, const DirectX::XMFLOAT4& color, 
	const string& texturehandle)
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (pat->isdead) {
			pat->Set(pos, scale, moveDistance,color, texturehandle);
			return;
		}
	}

	cubePool.emplace_back(std::make_unique<CubeParticle>(pos, scale, moveDistance, color, texturehandle));
}

void ParticleManager::CreateSphereParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color)
{
	for (unique_ptr<SphereParticle>& pat : spherePool) {
		if (pat->isdead) {
			pat->Set(pos, scale, moveDistance, color);
			return;
		}
	}

	spherePool.emplace_back(std::make_unique<SphereParticle>(pos, scale, moveDistance, color));
}

void ParticleManager::Update()
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->Update();
		}
	}
	for (unique_ptr<SphereParticle>& pat : spherePool) {
		if (!pat->isdead) {
			pat->Update();
		}
	}
}

void ParticleManager::Draw()
{
	BasicObjectPreDraw("DitherOutline",false);
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->part.DrawOutLine();
		}
	}

	for (unique_ptr<SphereParticle>& pat : spherePool) {
		if (!pat->isdead) {
			pat->part.DrawOutLine();
		}
	}
	
	BasicObjectPreDraw("DitherTransparent");
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (!pat->isdead) {
			pat->Draw();
		}
	}

	for (unique_ptr<SphereParticle>& pat : spherePool) {
		if (!pat->isdead) {
			pat->Draw();
		}
	}
}

CubeParticle::CubeParticle()
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));

	part.color_ = {
		218.0f / 255.0f,
		159.0f / 255.0f,
		64.0f / 255.0f,
		1 };

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

CubeParticle::CubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color, const string& texturehandle)
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));

	Set(pos,scale, moveDistance, color, texturehandle);

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

void CubeParticle::Set(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color, const string& texturehandle)
{
	part.SetTexture(TextureManager::Get()->GetTexture("white"));
	//テクスチャを何かしら使用するなら
	if (texturehandle != "") {
		part.SetTexture(TextureManager::Get()->GetTexture(texturehandle));
	}

	part.position ={
		pos.x,
		pos.y,
		pos.z
	};

	part.scale = {
		scale.x,
		scale.y,
		scale.z
	};

	part.color_ = {
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
	if (part.scale.x <= minScale ||
		part.scale.y <= minScale ||
		part.scale.z <= minScale
		)
	{
		isdead = true;
	}

	part.position.x += moveSpeed.x * TimeManager::deltaTime;
	part.position.y += moveSpeed.y * TimeManager::deltaTime;
	part.position.z += moveSpeed.z * TimeManager::deltaTime;

	part.scale.x -= shrinkSpeed.x * TimeManager::deltaTime;
	part.scale.y -= shrinkSpeed.y * TimeManager::deltaTime;
	part.scale.z -= shrinkSpeed.z * TimeManager::deltaTime;

	part.rotation.x += rotateSpeed.x * TimeManager::deltaTime;
	part.rotation.y += rotateSpeed.y * TimeManager::deltaTime;
	part.rotation.z += rotateSpeed.z * TimeManager::deltaTime;

	part.Update(*Camera::sCamera);
}

SphereParticle::SphereParticle()
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));
	part.SetModel(ModelManager::GetModel("Sphere"));

	part.color_ = {
		218.0f / 255.0f,
		159.0f / 255.0f,
		64.0f / 255.0f,
		1 };

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

SphereParticle::SphereParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance,const DirectX::XMFLOAT4& color)
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));
	part.SetModel(ModelManager::GetModel("Sphere"));

	Set(pos, scale, moveDistance, color);

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}


void SphereParticle::Set(const Vector3& pos, const Vector3& scale, const float& moveDistance, const DirectX::XMFLOAT4& color)
{
	part.position = {
		pos.x,
		pos.y,
		pos.z
	};

	part.scale = {
		scale.x,
		scale.y,
		scale.z
	};

	part.color_ = {
		color.x,
		color.y,
		color.z,
		color.w };

	moveSpeed.x = MathF::GetRand(-1.0f, 1.0f) * moveDistance;
	moveSpeed.y = MathF::GetRand(-1.0f, 1.0f) * moveDistance;
	moveSpeed.z = MathF::GetRand(-1.0f, 1.0f) * moveDistance;

	saveScale = scale;

	uptime.Start();
	downtime.Reset();

	isdead = false;
}

void SphereParticle::Update()
{
	uptime.Update();
	downtime.Update();

	if (downtime.GetEnd())
	{
		isdead = true; 
	}

	part.position.x += moveSpeed.x * TimeManager::deltaTime;
	part.position.y += moveSpeed.y * TimeManager::deltaTime;
	part.position.z += moveSpeed.z * TimeManager::deltaTime;

	if (uptime.GetEnd()) {
		if(!downtime.GetStarted())downtime.Start();
	}

	if (uptime.GetRun())
	{
		part.scale.x = TEasing::OutQuad(0.f, saveScale.x, uptime.GetTimeRate());
		part.scale.y = TEasing::OutQuad(0.f, saveScale.y, uptime.GetTimeRate());
		part.scale.z = TEasing::OutQuad(0.f, saveScale.z, uptime.GetTimeRate());
	}
	else
	{
		part.scale.x = TEasing::InBack(saveScale.x, 0.f, downtime.GetTimeRate());
		part.scale.y = TEasing::InBack(saveScale.y, 0.f, downtime.GetTimeRate());
		part.scale.z = TEasing::InBack(saveScale.z, 0.f, downtime.GetTimeRate());
	}

	part.Update(*Camera::sCamera);
}
