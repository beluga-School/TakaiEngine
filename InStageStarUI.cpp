#include "InStageStarUI.h"

void InStageStarUI::LoadResource()
{
	TextureManager::Load("Resources\\ui\\star2d.png", "star2d");
	TextureManager::Load("Resources\\ui\\star2d_blank.png", "star2d_blank");
}

void InStageStarUI::ChangeTexture(bool hoge, int32_t indent)
{
	if (indent < 0 || indent > 2)
	{
		return;
	}
	if (hoge)
	{
		stars[indent].SetTexture(*TextureManager::GetTexture("star2d"));
	}
	else
	{
		stars[indent].SetTexture(*TextureManager::GetTexture("star2d_blank"));
	}
}

void InStageStarUI::Initialize()
{
	mScale = { 0.5f ,0.5f };
	for (int32_t i = 0; i < 3; i++)
	{
		stars[i].SetSize(stars[i].GetInitSize() * mScale);
		stars[i].mPosition.x = mPos.x + (stars[i].mSize.x * i);
		stars[i].mPosition.y = mPos.y;
	}
}

void InStageStarUI::Update()
{
	for (int32_t i = 0; i < 3; i++)
	{
		stars[i].SetSize(stars[i].GetInitSize() * mScale);
		stars[i].mPosition.x = mPos.x + (stars[i].mSize.x * i);
		stars[i].mPosition.y = mPos.y;

		stars[i].Update();
	}
}

void InStageStarUI::Draw()
{
	for (int32_t i = 0; i < 3; i++)
	{
		stars[i].Draw();
	}
}
