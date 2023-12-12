#include "GetStar.h"
#include "Star.h"
#include "InstantDrawer.h"
#include "GameUIManager.h"
#include "EventManager.h"
#include "StarStringEvent.h"

void GetStar::Initialize()
{
	
}

void GetStar::Update()
{
	
}

void GetStar::Draw()
{
	
}

void GetStar::Start()
{
	Star::EventPopStar(eventName);
	EventManager::Get()->Start("StarStringEvent");

	GameUIManager::Get()->CDTimer.SetTime(20.0f);
	GameUIManager::Get()->CDTimer.Start();
}

void GetStar::End()
{
	//スターが未取得状態なら消す
	if (!Star::EventCheckStar(eventName)) {
		Star::EventVanishStar(eventName);
	}
}

bool GetStar::EndFlag()
{
	//スターがなくなったら成功で返す
	if (!Star::EventCheckStar(eventName)) {
		GameUIManager::Get()->CDTimer.Reset();
		return true;
	}

	//そうでないならタイマーを見て、タイマーが終了したなら成功で返す
	return GameUIManager::Get()->CDTimer.timer.GetReverseEnd();
}
