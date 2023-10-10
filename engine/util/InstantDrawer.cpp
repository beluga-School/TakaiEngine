#include "InstantDrawer.h"
#include "ClearDrawScreen.h"
#include "MathF.h"
#include <minmax.h>
//ビルボードがプレイヤーカメラ依存の設計になってるので、違和感消しのためにイベント中はビルボード表示をなくす用にする
//現在カメラ依存に修正できたら消していい
#include <EventManager.h>

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

void InstantDrawer::DrawGraph(const float& x, const float& y, float sizerateX, float sizerateY, const std::string& handle, const Anchor& anchor)
{
	sSprites.emplace_back();
	sSprites.back().SetTexture(*TextureManager::GetTexture(handle));
	sSprites.back().SetPos({ x,y });
	sSprites.back().SetSizeRate({ sizerateX, sizerateY });
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

void InstantDrawer::DrawGraph3D(const Vector3& pos, float width, float height, const std::string& handle)
{
	//includeで説明した通りイベント実行中なら消す
	if (EventManager::Get()->GetNowEvent() != nullptr) return;

	sBillboards.emplace_back();
	sBillboards.back().SetTexture(TextureManager::GetTexture(handle));
	sBillboards.back().position = pos;

	int32_t texWidth = (int32_t)TextureManager::GetTexture(handle)->GetMetaData()->width;
	int32_t texHeight = (int32_t)TextureManager::GetTexture(handle)->GetMetaData()->height;

	int32_t bigger = max(texWidth, texHeight);
	int32_t smaller = min(texWidth, texHeight);

	bigger /= smaller;
	smaller = 1;

	//横の方が大きいなら
	if (texWidth > texHeight)
	{
		sBillboards.back().scale = { (float)bigger * width,(float)smaller * height,1 };
	}
	//縦の方が大きいor同じなら
	else
	{
		sBillboards.back().scale = { (float)smaller * width,(float)bigger * height,1 };
	}
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
	BasicObjectPreDraw(PipelineManager::GetPipeLine("GroundToon"));

	for (auto& billboard : sBillboards)
	{
		billboard.Draw();
	}
}
