#include "InstantDrawer.h"
#include "ClearDrawScreen.h"

std::list<Sprite> InstantDrawer::sSprites;
std::list<Billboard> InstantDrawer::sBillboards;

void InstantDrawer::DrawInit()
{
	sSprites.clear();
	sBillboards.clear();
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

void InstantDrawer::DrawGraph3D(const Vector3& pos, const Vector3& scale, const std::string& handle)
{
	sBillboards.emplace_back();
	sBillboards.back().SetTexture(TextureManager::GetTexture(handle));
	sBillboards.back().position = pos;
	sBillboards.back().scale = scale;
}

void InstantDrawer::AllUpdate()
{
	for (auto& sprite : sSprites)
	{
		sprite.Update();
	}
	for (auto& billboard : sBillboards)
	{
		billboard.Update(*Camera::sCamera);
	}
}

void InstantDrawer::AllDraw2D()
{
	SpriteCommonBeginDraw();

	for (auto &sprite : sSprites)
	{
		sprite.Draw();
	}
}

void InstantDrawer::AllDraw3D()
{
	BasicObjectPreDraw(PipelineManager::GetPipeLine("Toon"));

	for (auto& billboard : sBillboards)
	{
		billboard.Draw();
	}
}
