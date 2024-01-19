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
		smokePool.emplace_back(std::make_unique<Smoke>());
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
	for (unique_ptr<Smoke>& pat : smokePool) {
		pat->isdead = true;
	}
}

void ParticleManager::CreateCubeParticle(const Vector3& pos, const Vector3& scale,
	const float& moveDistance, const DirectX::XMFLOAT4& color, 
	const string& texturehandle, PARTICLEPATTERN pattern_)
{
	for (unique_ptr<CubeParticle>& pat : cubePool) {
		if (pat->isdead) {
			pat->Set(pos, scale, moveDistance,color, texturehandle, pattern_);
			return;
		}
	}

	cubePool.emplace_back(std::make_unique<CubeParticle>(pos, scale, moveDistance, color, texturehandle, pattern_));
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

void ParticleManager::CreateSmoke(const Vector3& start, const Vector3& end, const Vector3& scale, float time, const Color& color, const EASEPATTERN& pattern)
{
	for (unique_ptr<Smoke>& pat : smokePool) {
		if (pat->isdead) {
			pat->Set(start,end,scale,time,color,pattern);
			return;
		}
	}

	smokePool.emplace_back(std::make_unique<Smoke>(start, end, scale, time, color, pattern));
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
	for (unique_ptr<Smoke>& pat : smokePool) {
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
	for (unique_ptr<Smoke>& pat : smokePool) {
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
	for (unique_ptr<Smoke>& pat : smokePool) {
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

CubeParticle::CubeParticle(const Vector3& pos, const Vector3& scale, const float& moveDistance, 
	const DirectX::XMFLOAT4& color, const string& texturehandle, PARTICLEPATTERN pattern_)
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));

	Set(pos,scale, moveDistance, color, texturehandle,pattern_);

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

void CubeParticle::Set(const Vector3& pos, const Vector3& scale, const float& moveDistance,
	const DirectX::XMFLOAT4& color, const string& texturehandle, PARTICLEPATTERN pattern_)
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
	
	saveScale = {
		scale.x,
		scale.y,
		scale.z
	};

	isdead = false;

	pattern = pattern_;

	lifeTimer.Start();
}

void CubeParticle::Update()
{
	lifeTimer.Update();

	if (lifeTimer.GetEnd())
	{
		isdead = true;
	}

	switch (pattern)
	{
	case PARTICLEPATTERN::NORMAL:
		part.position.x += moveSpeed.x * TimeManager::deltaTime;
		part.position.y += moveSpeed.y * TimeManager::deltaTime;
		part.position.z += moveSpeed.z * TimeManager::deltaTime;
	
		break;
	case PARTICLEPATTERN::DROP:
		if (lifeTimer.GetTimeRate() < 0.5f)
		{
			moveSpeed.y = TEasing::OutQuad(1.0f,1.5f,lifeTimer.GetTimeRate());
		}
		else
		{
			moveSpeed.y = TEasing::InQuad(1.5f,-20.f,lifeTimer.GetTimeRate());
		}

		part.position.x += moveSpeed.x * TimeManager::deltaTime;
		part.position.y += moveSpeed.y * TimeManager::deltaTime;
		part.position.z += moveSpeed.z * TimeManager::deltaTime;

		break;
	}

	part.scale = TEasing::OutQuad(saveScale, {0,0,0}, lifeTimer.GetTimeRate());

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

Smoke::Smoke()
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));
	part.SetModel(ModelManager::GetModel("smoke"));

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

Smoke::Smoke(const Vector3& start,
	const Vector3& end,
	const Vector3& scale,
	float time,
	const Color& color,
	const EASEPATTERN& pattern)
{
	part.Initialize();
	part.SetTexture(TextureManager::Get()->GetTexture("white"));
	part.SetModel(ModelManager::GetModel("smoke"));

	Set(start,end,scale,time,color,pattern);

	part.SetOutLineState({ 0,0,0,1 }, 0.05f);
}

void Smoke::Set(const Vector3& start_, 
	const Vector3& end_,
	const Vector3& scale_,
	float time_, 
	const Color& color_, 
	const EASEPATTERN& pattern_)
{
	start = start_;
	end = end_;
	part.position = start_;
	part.scale = scale_;
	part.color_ = color_.f4;
	pattern = pattern_;

	startScale = scale_ * 0.5f;
	maxScale = scale_;

	lifeTimer.mMaxTime = time_;

	upTimer.mMaxTime = time_ * 0.2f;
	stayTimer.mMaxTime = time_ * 0.2f;
	downTimer.mMaxTime = time_ * 0.6f;

	isdead = false;

	lifeTimer.Start();

	upTimer.Start();
	stayTimer.Reset();
	downTimer.Reset();

	part.rotation.x = MathF::GetRand(0.0f, 3.14f);
	part.rotation.y = MathF::GetRand(0.0f, 3.14f);
	part.rotation.z = MathF::GetRand(0.0f, 3.14f);
}

void Smoke::Update()
{
	upTimer.Update();
	stayTimer.Update();
	downTimer.Update();
	lifeTimer.Update();

	if (upTimer.GetNowEnd())
	{
		stayTimer.Start();
	}
	if (stayTimer.GetNowEnd())
	{
		downTimer.Start();
	}

	if (lifeTimer.GetEnd())
	{
		isdead = true;
	}

	switch (pattern)
	{
	case EASEPATTERN::INQUAD:
		if (upTimer.GetRun())
		{
			part.scale = TEasing::InQuad(startScale,maxScale, upTimer.GetTimeRate());
		}
		else
		{
			part.scale = TEasing::InQuad(maxScale, Vector3(0, 0, 0), downTimer.GetTimeRate());
		}
		break;
	case EASEPATTERN::OUTQUAD:
		if (upTimer.GetRun())
		{
			part.scale = TEasing::OutQuad(startScale, maxScale, upTimer.GetTimeRate());
		}
		else
		{
			part.scale = TEasing::OutQuad(maxScale, Vector3(0, 0, 0), downTimer.GetTimeRate());
		}break;
	case EASEPATTERN::INBACK:
		if (upTimer.GetRun())
		{
			part.scale = TEasing::InBack(startScale, maxScale, upTimer.GetTimeRate());
		}
		else
		{
			part.scale = TEasing::InBack(maxScale, Vector3(0, 0, 0), downTimer.GetTimeRate());
		}
		break;
	case EASEPATTERN::OUTBACK:
		if (upTimer.GetRun())
		{
			part.scale = TEasing::OutBack(startScale, maxScale, upTimer.GetTimeRate());
		}
		else
		{
			part.scale = TEasing::OutBack(maxScale, Vector3(0, 0, 0), downTimer.GetTimeRate());
		}break;
	}

	part.position = TEasing::InQuad(start, end, lifeTimer.GetTimeRate());
	part.Update(*Camera::sCamera);
}
