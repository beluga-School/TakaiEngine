#include "StarUI.h"
#include "Util.h"
#include "InstantDrawer.h"
#include "StageChanger.h"

void StarUI::LoadResource()
{
	TextureManager::Load("Resources\\UI\\star2d.png", "star2d");
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
	uibase.SetTexture("star2d");
	
	uibase.InitPos({Util::WIN_WIDTH + 400,80 }, { Util::WIN_WIDTH - 300,80 });
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
	std::vector<std::string> numbers
	{
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
	};

	InstantDrawer::DrawGraph(uibase.GetPos().x + 60, uibase.GetPos().y + 30, 1.5f, 0.6f, "uiback");
	InstantDrawer::DrawGraph(uibase.GetPos().x - 30, uibase.GetPos().y, 0.5f, 0.5f, "star2d");
	InstantDrawer::DrawGraph(uibase.GetPos().x + 60, uibase.GetPos().y, 0.5f, 0.5f, "x");
	InstantDrawer::DrawGraph(uibase.GetPos().x + 60, uibase.GetPos().y + 50, 1.0f, 1.0f, "starstring");
	
	if (count < 10) {
		InstantDrawer::DrawGraph(uibase.GetPos().x + 120, uibase.GetPos().y, 0.5f, 0.5f, numbers[count]);
	}
	else{
		int32_t ten = count / 10;
		int32_t one = count % 10;
		InstantDrawer::DrawGraph(uibase.GetPos().x + 120, uibase.GetPos().y, 0.5f, 0.5f, numbers[ten]);
		InstantDrawer::DrawGraph(uibase.GetPos().x + 180, uibase.GetPos().y, 0.5f, 0.5f, numbers[one]);
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

void StarUI::CountUp()
{
	count++;
}

void StarUI::CountDown()
{
	count--;
	//星の数の丸め(0以下にならないように)
	Util::Clamp(count, 0, 9999);
}

void StarUI::StarCount()
{
	count = 0;
	for (auto& star : StageChanger::Get()->mEntitys)
	{
		if (star->CheckTag(TagTable::Star))
		{
			count++;
		}
	}
}

void StarUI::Substitution(int32_t value)
{
	count = value;
}

int32_t StarUI::GetCount()
{
	return count;
}
