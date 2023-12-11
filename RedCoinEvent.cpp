#include "RedCoinEvent.h"
#include "RedCoin.h"

void RedCoinEvent::Initialize()
{

}

void RedCoinEvent::Update()
{

}

void RedCoinEvent::Draw()
{

}

void RedCoinEvent::Start()
{
	RedCoin::EventPopRedCoin(eventName);
}

void RedCoinEvent::End()
{

}

bool RedCoinEvent::EndFlag()
{
	return false;
}
