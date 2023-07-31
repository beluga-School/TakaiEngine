#include "Coin.h"
#include "ClearDrawScreen.h"

void Coin::Initialize()
{
	SetModel(ModelManager::GetModel("Coin"));
	SetTexture(TextureManager::GetTexture("white"));
}

void Coin::Update()
{
	Obj3d::Update(*Camera::sCamera);
}

void Coin::Draw()
{
	Obj3d::DrawMaterial();
}

void Coin::HitEffect()
{
	//2‰ñ–Ú‚Í“ü‚ç‚È‚¢‚æ‚¤‚É
	if (hit)return;

	hit = true;
}