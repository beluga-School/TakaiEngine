#include "CountDownTimer.h"
#include "Texture.h"
#include "InstantDrawer.h"
#include "player.h"

void CountDownTimer::LoadResource()
{
	TextureManager::Load("Resources\\UI\\twopoint.png", "twopoint");
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

void CountDownTimer::SetTime(float maxTime)
{
	timer.mMaxTime = maxTime;
}

void CountDownTimer::Start()
{
	mActive = true;

	timer.mElapsedTime = timer.mMaxTime;
	timer.ReverseStart();
}

void CountDownTimer::Reset()
{
	timer.Reset();
	mActive = false;
}

void CountDownTimer::Update()
{
	if (!mActive)return;

	if (timer.GetReverseEnd()) {
		mActive = false;
	}
	timer.Update();
}

void CountDownTimer::Draw()
{
	if (!mActive)return;

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

	int32_t ten = (int32_t)timer.mElapsedTime / 10;
	int32_t one = (int32_t)timer.mElapsedTime % 10;

	scalingTimer.Update();
	scalingTimer.Roop();

	float scale = TEasing::OutQuad(0.45f,0.55f, scalingTimer.GetTimeRate());

	Vector2 pos = { Util::WIN_WIDTH / 2 ,Util::WIN_HEIGHT / 2 - 100 };

	//タイマーを視覚的にするためのあれ
	float sizeX = TEasing::lerp(150, 0,1 - timer.GetTimeRate());

	Color barColor = Color(0.1f, 1.0f, 0.1f, 1.0f);
	barColor.f4.x = TEasing::lerp(0.1f, 1.0f, 1 - timer.GetTimeRate());
	barColor.f4.y = TEasing::lerp(1.0f, 0.1f, 1 - timer.GetTimeRate());

	//ゲージ
	InstantDrawer::DrawBox(pos.x - 80, pos.y + 70, 160, 25, Color(0.1f, 0.1f, 0.1f, 1.0f),InstantDrawer::Anchor::LEFT);
	InstantDrawer::DrawBox(pos.x - 75, pos.y + 70, sizeX, 15, barColor, InstantDrawer::Anchor::LEFT);
	
	//灰色背景
	InstantDrawer::DrawBox(pos.x, pos.y, 150, 100, Color(0.1f, 0.1f, 0.1f, 0.9f));

	InstantDrawer::DrawGraph(pos.x - 35, pos.y, scale, scale, numbers[ten]);
	InstantDrawer::DrawGraph(pos.x + 35, pos.y, scale, scale, numbers[one]);
}
