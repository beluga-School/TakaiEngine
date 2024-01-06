#include "RedCoinEvent.h"
#include "RedCoin.h"
#include "EventManager.h"
#include "Star.h"
#include "GameUIManager.h"

void RedCoinEvent::Initialize()
{

}

void RedCoinEvent::Update()
{
	//赤コインが全て取られたら
	if (!RedCoin::CheckEventRedCoin(eventName)) 
	{
	}
}

void RedCoinEvent::Draw()
{

}

void RedCoinEvent::Start()
{
	RedCoin::PopEventRedCoin(eventName);

	EventManager::Get()->Start("RedStringEvent");
}

void RedCoinEvent::End()
{
	//コインが残ってるなら消す
	if (RedCoin::CheckEventRedCoin(eventName)) {
		RedCoin::DeleteEventRedCoin(eventName);
	}
}

bool RedCoinEvent::EndFlag()
{
	//スターがなくなったら成功で返す
	if (!RedCoin::CheckEventRedCoin(eventName)) {
		GameUIManager::Get()->CDTimer.Reset();
		Star::EventPopStar(eventName);
		EventManager::Get()->Start("RedEventEnd");

		return true;
	}

	//そうでないならタイマーを見て、タイマーが終了したなら成功で返す
	return GameUIManager::Get()->CDTimer.timer.GetReverseEnd();
}
