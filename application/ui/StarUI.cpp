#include "StarUI.h"
#include "Util.h"
#include "InstantDrawer.h"
#include "StageChanger.h"
#include "MathF.h"

void StarUI::LoadResource()
{
	TextureManager::Load("Resources\\UI\\star2d.png", "star2d");
	TextureManager::Load("Resources\\UI\\starblank.png", "starblank");
	TextureManager::Load("Resources\\UI\\starshadow.png", "starshadow");
	TextureManager::Load("Resources\\UI\\uiback.png", "uiback");
	TextureManager::Load("Resources\\UI\\starstring.png", "starstring");

	TextureManager::Load("Resources\\UI\\x.png", "x");
	TextureManager::Load("Resources\\UI\\number1.png", "1");
	TextureManager::Load("Resources\\UI\\number2.png", "2");
	TextureManager::Load("Resources\\UI\\number3.png", "3");
	TextureManager::Load("Resources\\UI\\number4.png", "4");
	TextureManager::Load("Resources\\UI\\number5.png", "5");
	TextureManager::Load("Resources\\UI\\number6.png", "6");
	TextureManager::Load("Resources\\UI\\number7.png", "7");
	TextureManager::Load("Resources\\UI\\number8.png", "8");
	TextureManager::Load("Resources\\UI\\number9.png", "9");
	TextureManager::Load("Resources\\UI\\number0.png", "0");
}

void StarUI::Initialize()
{
	uibase.SetTexture("starblank");
	
	uibase.InitPos({-400,80 }, {0,80 });
	uibase.InitScale({ 0.5f,0.5f }, { 0.5f,0.5f });

	//初期化の際に出現フラグが立ってるなら出現させっぱなし
	if (uiAppearance)
	{
		uibase.Move(UIMove::START);
	}
}

void StarUI::Update()
{
	uibase.Update();
}

void StarUI::Draw()
{
	float invideX = 80;
	float shadowScale = 1.05f;
	Color backColor = { 0.1f,0.1f,0.1f,0.5f };

	for (auto& star : stars)
	{
		star->uiInfo.Update();
		if (star->uiInfo.get)
		{
			//下地描画
			InstantDrawer::DrawRotaGraph(uibase.GetPos().x + star->id * invideX,
				uibase.GetPos().y + 5, star->uiInfo.scale.x * shadowScale, star->uiInfo.scale.y * shadowScale,
				star->uiInfo.angle, "starshadow", backColor);
			//本描画
			InstantDrawer::DrawRotaGraph(uibase.GetPos().x + star->id * invideX,
				uibase.GetPos().y, star->uiInfo.scale.x, star->uiInfo.scale.y,
				star->uiInfo.angle, "star2d");
		}
		else
		{
			//下地描画
			InstantDrawer::DrawRotaGraph(uibase.GetPos().x + star->id * invideX,
				uibase.GetPos().y, star->uiInfo.scale.x, star->uiInfo.scale.y,
				star->uiInfo.angle, "starshadow", backColor);
			//本描画
			InstantDrawer::DrawRotaGraph(uibase.GetPos().x + star->id * invideX,
				uibase.GetPos().y, star->uiInfo.scale.x, star->uiInfo.scale.y, 
				star->uiInfo.angle,"starblank");
		}
	}
}

void StarUI::Move(UIMove uimove)
{
	switch (uimove)
	{
	case UIMove::START:
		uibase.Move(uimove);
		break;
	case UIMove::END:
		uibase.Move(uimove);
		break;
	}
}

void StarUI::StarCount()
{
	stars.clear();
	for (auto& ent : StageChanger::Get()->mEntitys)
	{
		if (ent->CheckTag(TagTable::Star))
		{
			Star* star = static_cast<Star*>(ent.get());
			stars.push_back(star);
		}
	}
}

bool StarUI::CheckAllCollect()
{
	//どれか一つでも未入手ならfalse
	for (auto& star : stars)
	{
		if (!star->uiInfo.get)return false;
	}
	//全部通ってたらtrue
	return true;
}
