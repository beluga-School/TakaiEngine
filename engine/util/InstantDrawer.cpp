#include "InstantDrawer.h"

std::list<Sprite> InstantDrawer::sSprites;

void InstantDrawer::DrawInit()
{
	sSprites.clear();
}

void InstantDrawer::DrawBox(const float& x, const float& y, const float& width, const float& height, const Color& color, const Anchor& anchor)
{
	sSprites.emplace_back();
	sSprites.back().SetTexture(*TextureManager::GetTexture("white"));
	sSprites.back().SetPos({ x,y });
	sSprites.back().SetSize({ width, height });
	sSprites.back().SetColor(color);
	switch (anchor)
	{
	case InstantDrawer::Anchor::LEFT:
		sSprites.back().SetAnchor({ 0,0.5f });
		break;
	case InstantDrawer::Anchor::CENTER:
		sSprites.back().SetAnchor({ 0.5f,0.5f });
		break;
	case InstantDrawer::Anchor::RIGHT:
		sSprites.back().SetAnchor({ 1.0f,0.5f });
		break;
	}
}

void InstantDrawer::AllUpdate()
{
	for (auto& sprite : sSprites)
	{
		sprite.Update();
	}
}

void InstantDrawer::AllDraw()
{
	for (auto &sprite : sSprites)
	{
		sprite.Draw();
	}
}
