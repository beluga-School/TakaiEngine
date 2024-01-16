#include "Grass.h"
#include "MathF.h"

void Grass::LoadResource()
{
	TextureManager::Load("Resources\\movegrass.png", "movegrass");
}

void Grass::Initialize()
{
	poligon1.SetModel(ModelManager::GetModel("plate"));
	poligon1.SetTexture(TextureManager::GetTexture("movegrass"));
	poligon2.SetModel(ModelManager::GetModel("plate"));
	poligon2.SetTexture(TextureManager::GetTexture("movegrass"));
}

void Grass::Update()
{
	timer.Update();
	timer.Roop();

	poligon1.position = position;
	poligon1.position.y -= 0.2f * (1.0f-timer.GetTimeRate());
	poligon2.position = position;
	poligon2.position.y -= 0.2f * (1.0f - timer.GetTimeRate());

	poligon2.rotation.y = MathF::AngleConvRad(90);

	poligon1.scale.y = TEasing::InQuad(0.8f,1.f, timer.GetTimeRate());
	poligon2.scale.y = TEasing::InQuad(0.8f,1.f, timer.GetTimeRate());

	poligon1.Update(*Camera::sCamera);
	poligon2.Update(*Camera::sCamera);
}

void Grass::Draw()
{
	poligon1.Draw();
	poligon2.Draw();
}
