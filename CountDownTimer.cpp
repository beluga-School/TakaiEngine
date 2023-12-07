#include "CountDownTimer.h"
#include "Texture.h"
#include "InstantDrawer.h"

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

	InstantDrawer::DrawGraph(200, 100, 0.5f, 0.5f, numbers[ten]);
	InstantDrawer::DrawGraph(200 + 70, 100, 0.5f, 0.5f, numbers[one]);

}
